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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "device.h"
#include "logger.h"

static struct bus_driver root_device_driver = {
    .init     = NULL,
    .init_dev = NULL
};

static struct bus root_device = {
    .name   = "root_dev",
    .parent = NULL,
    .driver = &root_device_driver
};

// Static functions to manage devices
static int init_device(struct device *dev);
static int init_bus(struct bus *bus);
static struct device* find_device(struct bus *bus, const char *name);

static int init_device(struct device *dev)
{
    int ret;

    if (dev->driver->init && (ret = dev->driver->init(dev)) != 0) {
        log_error("init dev %s (%d)",  dev->name, ret);
        return ret;
    }
    return 0;
}

static int init_bus(struct bus *bus)
{
    int ret;
    unsigned int i;

    // Init current bus
    if (bus->driver->init && (ret = bus->driver->init(bus)) != 0) {
        log_error("init bus %s (%d)",  bus->name, ret);
        return ret;
    }
    // Init current bus devices
    for (i = 0; i < bus->dev_count; i++) {
        bus->c_dev[i].parent = bus;
        if ((ret = init_device(&bus->c_dev[i])) != 0) {
            return ret;
        }
        if (bus->driver->init_dev && (ret = bus->driver->init_dev(bus, &bus->c_dev[i])) != 0) {
            log_error("init bus dev %s (%d)",  bus->name, ret);
            return ret;
        }
    }
    // Init current bus buses
    for (i = 0; i < bus->bus_count; i++) {
        bus->c_bus[i].parent = bus;
        if ((ret = init_bus(&bus->c_bus[i])) != 0) {
            return ret;
        }
    }
    return 0;
}

int init_platform_devices(struct device *devices, unsigned int dev_count, struct bus *buses, unsigned int bus_count)
{
    // Init root device tree
    root_device.c_dev = devices;
    root_device.dev_count = dev_count;
    root_device.c_bus = buses;
    root_device.bus_count = bus_count;

    return init_bus(&root_device);
}

static struct device* find_device(struct bus *bus, const char *name)
{
    unsigned int i;
    struct device *dev;

    for (i = 0; i < bus->dev_count; i++) {
        if (!strcmp(bus->c_dev[i].name, name)) {
            // Device found
            return &bus->c_dev[i];
        }
    }
    // Search in child buses
    for (i = 0; i < bus->bus_count; i++) {
        if ((dev = find_device(&bus->c_bus[i], name))) {
            // Device found
            return dev;
        }
    }
    return NULL;
}

struct device* get_device(const char *name)
{
    return find_device(&root_device, name);
}

static void list_device(struct bus *bus, unsigned int level)
{
    unsigned int i;

    log_info("(%d) %s (%s)", level++, bus->name, bus->parent->name);

    for (i = 0; i < bus->bus_count; i++) {
        list_device(&bus->c_bus[i], level);
    }
    for (i = 0; i < bus->dev_count; i++) {
        log_info("(%d) %s (%s)", level, bus->c_dev[i].name, bus->c_dev[i].parent->name);
    }
}

void list_devices(void)
{
    return list_device(&root_device, 0);
}
