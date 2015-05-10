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

#include "board_config.h"

#include <stdint.h>
#include <stdbool.h>
#include "pic24f_gpio.h"
#include "device.h"
#include "logger.h"

// Driver static functions
static int gpio_init(struct device *dev);

struct driver pic24f_gpio_driver = {
    .init = gpio_init
};

static int gpio_init(struct device *dev)
{
    return 0;
}

void gpio_configure(uint8_t pin, bool input, bool drain)
{
    uint16_t offset = ((pin & 0xf0) >> 2);
    uint16_t mask = (1 << (pin & 0x0f));

    if (input) {
        *(&TRISA + offset) |= mask;
    } else {
        *(&TRISA + offset) &= ~mask;
    }
    // if (drain) {
    //     *(&ODCA+offset) |= mask;
    // } else {
    //     *(&ODCA+offset) &= ~mask;
    // }
}

bool gpio_get(uint8_t pin)
{
    uint16_t offset = ((pin & 0xf0) >> 2);
    uint16_t mask = (1 << (pin & 0x0f));

    return !!(*(&PORTA + offset) & mask);
}

void gpio_set(uint8_t pin, bool state)
{
    uint16_t offset = ((pin & 0xf0) >> 2);
    uint16_t mask = (1 << (pin & 0x0f));

    if (state) {
        *(&LATA + offset) |= mask;
    } else {
        *(&LATA + offset) &= ~mask;
    }
}

void gpio_toggle(uint8_t pin)
{
    uint16_t offset = ((pin & 0xf0) >> 2);
    uint16_t mask = (1 << (pin & 0x0f));

    if (*(&PORTA + offset) & mask) {
        *(&LATA + offset) &= ~mask;
    } else {
        *(&LATA + offset) |= mask;
    }
}
