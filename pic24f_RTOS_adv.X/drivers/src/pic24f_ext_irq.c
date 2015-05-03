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

// Compile this driver only if needed
#if defined(EXT_IRQ_INT0) || defined(EXT_IRQ_INT1) || defined(EXT_IRQ_INT2)

#include <stdint.h>
#include <errno.h>
#include "pic24f_ext_irq.h"
#include "device.h"
#include "logger.h"

struct driver pic24f_ext_irq_driver = {
    .init = NULL
};

struct ext_irq_info {
    void (*callback)(void *priv);
    void *priv;
    uint8_t is_init;
};

// Driver static variables
static struct ext_irq_info ext_irq_drv_data[EXT_IRQ_PIN_COUNT] = { { .is_init = 0 } };

int register_ext_irq(EXT_IRQ_PIN pin, EXT_IRQ_TYPE type, void (*callback)(void *priv), void *priv, unsigned int dpl)
{
    // ASSERT(dpl <= 7);
    // ASSERT(pin < EXT_IRQ_PIN_COUNT);

    // TODO: interrupt lock
    if (ext_irq_drv_data[pin].is_init) {
        return -EACCESS;
    }
    ext_irq_drv_data[pin].callback = callback;
    ext_irq_drv_data[pin].priv = priv;
    ext_irq_drv_data[pin].is_init = 1;

    switch (pin) {
#if defined(EXT_IRQ_INT0)
    case INT0_PIN:
        IFS0bits.INT0IF = 0;
        IPC0bits.INT0IP = dpl;
        IEC0bits.INT0IE = 1;
        INTCON2bits.INT0EP = !!type;
        break;
#endif
#if defined(EXT_IRQ_INT1)
    case INT1_PIN:
        IFS1bits.INT1IF = 0;
        IPC5bits.INT1IP = dpl;
        INTCON2bits.INT1EP = !!type;
        IEC1bits.INT1IE = 1;
        break;
#endif
#if defined(EXT_IRQ_INT2)
    case INT2_PIN:
        IFS1bits.INT2IF = 0;
        IPC7bits.INT2IP = dpl;
        IEC1bits.INT2IE = 1;
        INTCON2bits.INT2EP = !!type;
        break;
#endif
    default:
        log_error("wrong ext_irq pin %d", pin);
        return -EINVAL;
    }
    return 0;
}

int unregister_ext_irq(EXT_IRQ_PIN pin)
{
    // ASSERT(pin < EXT_IRQ_PIN_COUNT);

    // TODO: interrupt lock
    if (!(ext_irq_drv_data[pin].is_init)) {
        return -EINVAL;
    }
    switch (pin) {
#if defined(EXT_IRQ_INT0)
    case INT0_PIN:
        IEC0bits.INT0IE = 0;
        break;
#endif
#if defined(EXT_IRQ_INT1)
    case INT1_PIN:
        IEC1bits.INT1IE = 0;
        break;
#endif
#if defined(EXT_IRQ_INT2)
    case INT2_PIN:
        IEC1bits.INT2IE = 0;
        break;
#endif
    default:
        return -EINVAL;
    }
    ext_irq_drv_data[pin].is_init = 0;
    return 0;
}

#if defined(EXT_IRQ_INT0)
void __attribute__((__interrupt__, auto_psv)) _INT0Interrupt(void)
{
    // Clear the INT0 interrupt flag
    IFS0bits.INT0IF = 0;
    if (ext_irq_drv_data[INT0_PIN].is_init) {
        ext_irq_drv_data[INT0_PIN].callback(ext_irq_drv_data[INT0_PIN].priv);
    } else {
        log_error("int0 spurious irq");
    }
}
#endif

#if defined(EXT_IRQ_INT1)
void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)
{
    // Clear the INT1 interrupt flag
    IFS1bits.INT1IF = 0;
    if (ext_irq_drv_data[INT1_PIN].is_init) {
        ext_irq_drv_data[INT1_PIN].callback(ext_irq_drv_data[INT1_PIN].priv);
    } else {
        log_error("int1 spurious irq");
    }
}
#endif

#if defined(EXT_IRQ_INT2)
void __attribute__((__interrupt__, auto_psv)) _INT2Interrupt(void)
{
    // Clear the INT2 interrupt flag
    IFS1bits.INT2IF = 0;
    if (ext_irq_drv_data[INT2_PIN].is_init) {
        ext_irq_drv_data[INT2_PIN].callback(ext_irq_drv_data[INT2_PIN].priv);
    } else {
        log_error("int2 spurious irq");
    }
}
#endif

#endif
