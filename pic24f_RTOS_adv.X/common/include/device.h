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

#ifndef _DEVICE_H
#define _DEVICE_H

#include "device_ids.h"

#define DECLARE_BUS_DEVICES(type, count) \
    .dev_count = count, \
    .c_dev = (struct device*)(struct type[])
#define DECLARE_BUS_DEVICES_NULL  .c_dev = NULL, .dev_count = 0

#define DECLARE_BUS_BUSES(type, count) \
    .bus_count = count, \
    .c_bus = (struct bus*)(struct type[])
#define DECLARE_BUS_BUSES_NULL    .c_bus = NULL, .bus_count = 0

struct device;
struct bus;
struct driver;
struct bus_driver;

int init_platform_devices(struct device *devices, unsigned int dev_count, struct bus *buses, unsigned int bus_count);

void setup_device(void);

struct device {
    struct bus *parent;
    const int id;
    void *priv;
    const struct driver *driver;
};

struct bus {
    struct bus *parent;
    const int id;
    void *priv;
    const struct bus_driver *driver;

    struct bus    *c_bus;
    unsigned int bus_count;
    struct device *c_dev;
    unsigned int dev_count;
};

struct driver {
    int (*init)(struct device *dev);
};

struct bus_driver {
    int (*init)(struct bus *dev);
    int (*init_dev)(struct bus *bus, struct device *dev);
};

struct device* get_device(const int id);

#ifdef DEBUG
void list_devices(void);
#endif

#endif
