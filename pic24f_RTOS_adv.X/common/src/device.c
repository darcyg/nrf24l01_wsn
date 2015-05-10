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
#include "device.h"
#include "logger.h"

static struct bus_driver root_device_driver = {
    .init     = NULL,
    .init_dev = NULL
};

static struct bus root_device = {
    .id     = DEV_ROOT,
    .parent = NULL,
    .driver = &root_device_driver
};

// Static functions to manage devices
static int init_device(struct device *dev);
static int init_bus(struct bus *bus);
static struct device* find_device(struct bus *bus, const int id);

static int init_device(struct device *dev)
{
    int ret;

    if (dev->driver->init && (ret = dev->driver->init(dev)) != 0) {
        log_error("init dev %d (%d)",  dev->id, ret);
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
        log_error("init bus %d (%d)",  bus->id, ret);
        return ret;
    }
    // Init current bus devices
    for (i = 0; i < bus->dev_count; i++) {
        bus->c_dev[i].parent = bus;
        if ((ret = init_device(&bus->c_dev[i])) != 0) {
            return ret;
        }
        if (bus->driver->init_dev && (ret = bus->driver->init_dev(bus, &bus->c_dev[i])) != 0) {
            log_error("init bus dev %d (%d)",  bus->id, ret);
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

static struct device* find_device(struct bus *bus, const int id)
{
    unsigned int i;
    struct device *dev;

    for (i = 0; i < bus->dev_count; i++) {
        if (bus->c_dev[i].id == id) {
            // Device found
            return &bus->c_dev[i];
        }
    }
    // Search in child buses
    for (i = 0; i < bus->bus_count; i++) {
        if ((dev = find_device(&bus->c_bus[i], id))) {
            // Device found
            return dev;
        }
    }
    return NULL;
}

struct device* get_device(const int id)
{
    return find_device(&root_device, id);
}

#ifdef DEBUG
static void list_device(struct bus *bus, unsigned int level)
{
    unsigned int i;

    log_debug("(%d) %d (%d)", level, bus->id, bus->parent->id);
    level++;

    for (i = 0; i < bus->bus_count; i++) {
        list_device(&bus->c_bus[i], level);
    }
    for (i = 0; i < bus->dev_count; i++) {
        log_debug("(%d) %d (%d)", level, bus->c_dev[i].id, bus->c_dev[i].parent->id);
    }
}

void list_devices(void)
{
    return list_device(&root_device, 0);
}
#endif
