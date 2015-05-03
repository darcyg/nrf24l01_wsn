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

#ifndef _EXT_IRQ_H
#define _EXT_IRQ_H

typedef enum {
    RISING_EDGE = 0,
    FALLING_EDGE,
    EXT_IRQ_TYPE_COUNT
} EXT_IRQ_TYPE;

typedef enum {
    INT0_PIN = 0,
    INT1_PIN,
    INT2_PIN,
    EXT_IRQ_PIN_COUNT
} EXT_IRQ_PIN;

extern struct driver pic24f_ext_irq_driver;

int register_ext_irq(EXT_IRQ_PIN pin, EXT_IRQ_TYPE type, void (*callback)(void *priv), void *priv, unsigned int dpl);

int unregister_ext_irq(EXT_IRQ_PIN pin);

#endif
