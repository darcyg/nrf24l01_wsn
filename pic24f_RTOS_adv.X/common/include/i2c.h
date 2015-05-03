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

#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include "device.h"

struct i2c_msg;
struct i2c_device;

struct i2c_driver {
    struct bus_driver driver;
    // Transfer function
    int (*xfer)(struct i2c_device *i2c, struct i2c_msg *msg);
};

struct i2c_device {
    struct device dev;
    uint8_t address;
    // TODO: add speed, mode etc...
};

struct i2c_msg {
    struct i2c_device *dev;
    void (*complete)(void *priv);
    void  *priv;
    struct i2c_msg *next;
};

#endif
