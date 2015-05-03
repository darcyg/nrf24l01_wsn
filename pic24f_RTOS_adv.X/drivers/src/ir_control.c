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

#include "HardwareProfile.h"
#include "ir_control.h"
#include "ir_control_defs.h"
#include "pic24f_gpio.h"
#include "device.h"

#define TOPBIT 0x80000000

typedef void (*ir_handler) (uint32_t data, unsigned int nbits);

static int ir_ctl_init(struct device *dev);

struct driver pic24f_ir_ctl_driver = {
    .init = ir_ctl_init
};

// struct ir_cmd {
//     uint32_t     data; // Data to send
//     unsigned int nbits; // Number of bytes to send
//     unsigned int step;
//     ir_handler   handler; // Function handler to send data
// };
// 
// struct ir_cmd ir_cmd_queue[16];
// unsigned int ir_head;
// unsigned int ir_tail;
// struct timer ir_timer;

void ir_handleRC5(uint32_t data, unsigned int nbits);

void ir_set_frequency(unsigned int freq)
{

}

int ir_ctl_init(struct device *dev)
{
//     // timer_set(&ir_timer, TIMER1_TICK_PER_SECOND/IR_LED_TX_PER_SEC);
//     memset(ir_cmd_queue, 0, 16*sizeof(struct ir_cmd));
// 
//     ir_head = 0;
//     ir_tail = 0;

    gpio_configure(((struct pic24f_ir_ctl_priv*)dev->priv)->pin_led, 0, 0);

    // Initialize Output Compare Module
    OC1R = 0;
    OC1RS = 421;
    OC1CON2 = 0x001F;   // Sync with This OC module
    OC1CON1 = 0x1C08;   // Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1)
    //OC1CON1bits.OCTSEL = 7; // Select system clock
    //OC1CON1bits.OCM = 0b110;
    OC1CON1 = OC1CON1 | 0x0006;   // Mode 6, Edge-aligned PWM Mode

    return 0;
}

void ir_push_queue(uint32_t data, unsigned int nbits, ir_handler handler) {
//     ir_cmd_queue[ir_head].data = data;
//     ir_cmd_queue[ir_head].nbits = nbits;
//     ir_cmd_queue[ir_head].handler = handler;
// 
//     if(ir_head == ir_tail) {
//         // Reset timer if ir module is not used for a long time
//         timer_set_ready(&ir_timer, TIMER1_TICK_PER_SECOND/IR_LED_TX_PER_SEC);
//     }
//     ir_head = (ir_head+1)&0x000f;
}

void ir_sendRC5(uint32_t data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleRC5);
}

// void ir_periodic()
// {
//     if(ir_head != ir_tail && timer_expired(&ir_timer)) {
//         timer_reset(&ir_timer);
//         ir_cmd_queue[ir_tail].handler(ir_cmd_queue[ir_tail].data, ir_cmd_queue[ir_tail].nbits);
// 
//         ir_tail = (ir_tail+1)&0x000f;
//     }
// }

// static void mark(unsigned int time)
// {
//     OC1R = 210;
//     __delay32(((uint32_t)time)*16UL);
// }
// static void space(unsigned int time)
// {
//     OC1R = 0;
//     __delay32(((uint32_t)time)*16UL);
// }

// static void ir_handleRC5(uint32_t data, unsigned int nbits, unsigned int step)
// {
//   int i;
//   // enableIROut(36);
//   data = data << (32 - nbits);
//   mark(RC5_T1); // First start bit
//   space(RC5_T1); // Second start bit
//   mark(RC5_T1); // Second start bit
//   for (i = 0; i < nbits; i++) {
//     if (data & TOPBIT) {
//       space(RC5_T1); // 1 is space, then mark
//       mark(RC5_T1);
//     } 
//     else {
//       mark(RC5_T1);
//       space(RC5_T1);
//     }
//     data <<= 1;
//   }
//   space(0); // Turn off at end
// }
