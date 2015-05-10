/*
 * pic24f RTOS based OS
 * Copyright (C) 2015 <spiriou31@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Configuration includes
#include "board_config.h"
#include "FreeRTOS.h"
// Common includes
#include "device.h"
#include "timers.h"
#include "logger.h"
// Drivers
#include "pic24f_gpio.h"
#include "ir_control.h"
#include "ir_control_defs.h"

#define TOPBIT         0x80000000
#define T2_US_TO_TICKS 2
#define OC_HIGH_VALUE  210
#define OC_LOW_VALUE   0
#define TIMEOUT_DELAY  120

static int ir_ctl_init(struct device *dev);

struct driver pic24f_ir_ctl_driver = {
    .init = ir_ctl_init
};

static void ir_handleRC5(struct ir_cmd *cmd);
static void ir_handleCanal(struct ir_cmd *cmd);
static void ir_handleNEC(struct ir_cmd *cmd);
static void ir_timeout_callback(void *priv);

// For now, only one instance of this driver => use static
static struct pic24f_ir_ctl_priv *dev_priv;

static int ir_ctl_init(struct device *dev)
{
    dev_priv = (struct pic24f_ir_ctl_priv*)dev->priv;
    // Init IR driver circular buffer for cmds
    dev_priv->ir_head = 0;
    dev_priv->ir_tail = 0;

    // Configure IR pin as output
    gpio_configure(dev_priv->pin_led, 0, 0);

    // Initialize Output Compare module
    OC1R = 0;
    OC1RS = 421;
    OC1CON2 = 0x001F;   // Sync with This OC module
    OC1CON1 = 0x1C08;   // Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1)
    OC1CON1 = OC1CON1 | 0x0006;   // Mode 6, Edge-aligned PWM Mode

    // Init timer 2 16bits module
    IEC0bits.T2IE = 0;
    T3CON = 0x0000; // Disable Timer 3 (Timer 2 is a 16bits timer)
    TMR2 = 0;
    T2CON = 0x8010; // Timer 2 clock prescale = 8
    // Set same priority as kernel IRQs to save stack
    IPC1bits.T2IP = configKERNEL_INTERRUPT_PRIORITY;
    IFS0bits.T2IF = 0;   //reset interrupt flag

    // Create timeout timer
    dev_priv->ir_timer = xTimerCreate("ir", (TickType_t)pdMS_TO_TICKS(TIMEOUT_DELAY), pdFALSE,
                            dev_priv, ir_timeout_callback);
    return 0;
}

static void ir_timeout_callback(void *priv)
{
    // TODO: use struct pic24f_ir_ctl_priv *ir_priv = (struct pic24f_ir_ctl_priv*)priv;
    struct pic24f_ir_ctl_priv *ir_priv = dev_priv;

    portENTER_CRITICAL();
    ir_priv->ir_tail = (ir_priv->ir_tail+1) & (IR_QUEUE_SIZE-1);
    if(ir_priv->ir_head != ir_priv->ir_tail) { // Another ir_cmd on circ buffer
        // Restart timer for next timeout
        xTimerStart(ir_priv->ir_timer, pdMS_TO_TICKS(TIMEOUT_DELAY));
        ir_priv->cur_ir_cmd = &ir_priv->ir_cmd_queue[ir_priv->ir_tail];
        ir_priv->cur_ir_cmd->handler((struct ir_cmd*)ir_priv->cur_ir_cmd);
    }
    portEXIT_CRITICAL();
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    dev_priv->cur_ir_cmd->handler((struct ir_cmd*)dev_priv->cur_ir_cmd);
    IFS0bits.T2IF = 0; // Reset interrupt flag
}

void ir_push_queue(uint32_t data, unsigned int nbits, ir_handler handler) {
    data = data << (32 - nbits);
    // Critical section
    portENTER_CRITICAL();
    dev_priv->ir_cmd_queue[dev_priv->ir_head].data = data;
    dev_priv->ir_cmd_queue[dev_priv->ir_head].nbits = nbits;
    dev_priv->ir_cmd_queue[dev_priv->ir_head].step = 0;
    dev_priv->ir_cmd_queue[dev_priv->ir_head].handler = handler;

    if(dev_priv->ir_head == dev_priv->ir_tail) {
        // Restart IR module and Timer 2
        // Start timeout timer
        xTimerStart(dev_priv->ir_timer, pdMS_TO_TICKS(TIMEOUT_DELAY));
        dev_priv->cur_ir_cmd = &dev_priv->ir_cmd_queue[dev_priv->ir_head];
        dev_priv->cur_ir_cmd->handler((struct ir_cmd*)dev_priv->cur_ir_cmd);
    }
    dev_priv->ir_head = (dev_priv->ir_head+1) & (IR_QUEUE_SIZE-1);
    // End critical
    portEXIT_CRITICAL();
}

void ir_sendRC5(uint32_t data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleRC5);
}

void ir_sendCanal(uint32_t data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleCanal);
}

void ir_sendNEC(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleNEC);
}

#define ir_mark(time) \
    do { OC1R = OC_HIGH_VALUE; \
    PR2 = time*T2_US_TO_TICKS; } while(0)

#define ir_space(time) \
    do { OC1R = OC_LOW_VALUE; \
    PR2 = time*T2_US_TO_TICKS; } while(0)

#define ir_toggle(time) \
    do { OC1R ^= OC_HIGH_VALUE; \
    PR2 = time*T2_US_TO_TICKS; } while(0)

#define ir_stop() \
    do { OC1R = OC_LOW_VALUE; } while(0)

static void ir_handleRC5(struct ir_cmd *cmd)
{
    if (likely(cmd->step > 0)) {
        if (cmd->step & 1) {
            // Read next data bit
            if (likely(cmd->step <= (cmd->nbits*2))) {
                if (cmd->data & TOPBIT) {
                    ir_space(RC5_T1); // 1 is space, then mark
                } else {
                    ir_mark(RC5_T1);
                }
                cmd->data <<= 1;
            } else {
                // IR frame done
                ir_stop(); // Turn off at end
                // Stop timer 2 and wait for FreeRTOS timer timeout
                IEC0bits.T2IE = 0;
            }
        } else {
            // Just toggle IR LED
            ir_toggle(RC5_T1);
        }
    } else { // cmd->step = 0, start bit
        ir_mark(RC5_T1); // First start bit
        // Start timer2
        TMR2 = 0;
        IFS0bits.T2IF = 0; //reset interrupt flag
        IEC0bits.T2IE = 1;
    }

    cmd->step++;
}

static void ir_handleCanal(struct ir_cmd *cmd)
{
    if (cmd->step & 1) {
        // Just toggle IR LED
        ir_toggle(CANAL_T1);
    } else {
        // Read next data bit
        if (likely(cmd->step < (cmd->nbits*2))) {
            if (cmd->data & TOPBIT) {
                ir_mark(CANAL_T1);
            } else {
                ir_space(CANAL_T1);
            }
            cmd->data <<= 1;

            if (unlikely(cmd->step == 0)) {
                // Start timer2
                PR2 -= 40*T2_US_TO_TICKS; // Compensate 40us latency (if not, receiver fails)
                TMR2 = 0;
                IFS0bits.T2IF = 0; //reset interrupt flag
                IEC0bits.T2IE = 1;
            }
        } else {
            // IR frame done
            ir_stop(); // Turn off at end
            IEC0bits.T2IE = 0;
        }
    }

    cmd->step++;
}

static void ir_handleNEC(struct ir_cmd *cmd)
{
    static unsigned int space_time = 0;

    if (likely(cmd->step >= 2)) {
        if (cmd->step & 1) {
            if(unlikely(cmd->step-2 > (cmd->nbits*2))) {
                ir_stop(); // Turn off at end
                // Stop timer 2 and wait for FreeRTOS timer timeout
                IEC0bits.T2IE = 0;
            } else {
                // Space
                ir_space(space_time);
            }
        } else {
            // Read next data bit
            if (likely(cmd->step-2 < (cmd->nbits*2))) {
                ir_mark(NEC_BIT_MARK);
                if (cmd->data & TOPBIT) {
                    space_time = NEC_ONE_SPACE;
                } else {
                    space_time = NEC_ZERO_SPACE;
                }
                cmd->data <<= 1;
            } else {
                // IR frame done
                ir_mark(NEC_BIT_MARK);
            }
        }
    } else if (cmd->step > 0) { // cmd->step = 0, start bit
        ir_space(NEC_HDR_SPACE);
    } else {
        ir_mark(NEC_HDR_MARK);
        // Start timer2
        TMR2 = 0;
        IFS0bits.T2IF = 0; //reset interrupt flag
        IEC0bits.T2IE = 1;
    }

    cmd->step++;
}
