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

#include "pic24f_i2c.h"
#include "logger.h"

static int pic24f_i2c2_init(struct bus *dev);
static int pic24f_i2c_init_dev(struct bus *bus, struct device *dev);
static int pic24f_i2c_transfer(struct i2c_device *i2c, struct i2c_msg *msg);

struct i2c_driver pic24f_i2c2_driver = {
    .driver         = {
        .init       = pic24f_i2c2_init,
        .init_dev   = pic24f_i2c_init_dev
    },
    .xfer           = pic24f_i2c_transfer
};

static int pic24f_i2c2_init(struct bus *dev)
{
    log_info("init i2c master %s", dev->name);
    return 0;
}

static int pic24f_i2c_init_dev(struct bus *bus, struct device *dev)
{
    log_info("init i2c dev %s (%s)", dev->name, bus->name);
    return 0;
}

static int pic24f_i2c_transfer(struct i2c_device *i2c, struct i2c_msg *msg)
{
    log_info("xfer i2c dev %s", i2c->dev.name);
    return 0;
}
