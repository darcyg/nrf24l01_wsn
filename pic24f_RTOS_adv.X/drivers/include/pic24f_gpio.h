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

#ifndef PIC24F_GPIO_H
#define PIC24F_GPIO_H

#include <stdbool.h>

#define GPIO_PORTA 0x00
#define GPIO_PORTB 0x10
#define GPIO_PORTC 0x20

extern struct driver pic24f_gpio_driver;

void gpio_configure(uint8_t pin, bool input, bool drain);

bool gpio_get(uint8_t pin);

void gpio_set(uint8_t pin, bool state);

void gpio_toggle(uint8_t pin);

#endif
