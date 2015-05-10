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

#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include <string.h>
#include "device.h"

// SPI modes from linux kernel
#define SPI_CPHA        0x01                    /* clock phase */
#define SPI_CPOL        0x02                    /* clock polarity */
#define SPI_MODE_0      (0 | 0)                 /* (original MicroWire) */
#define SPI_MODE_1      (0 | SPI_CPHA)
#define SPI_MODE_2      (SPI_CPOL | 0)
#define SPI_MODE_3      (SPI_CPOL | SPI_CPHA)
#define SPI_CS_HIGH     0x04                    /* chipselect active high? */
#define SPI_LSB_FIRST   0x08                    /* per-word bits-on-wire */
#define SPI_3WIRE       0x10                    /* SI/SO signals shared */

struct spi_req;
struct spi_msg;
struct spi_device;

struct spi_driver {
    struct bus_driver driver;
    // Transfer function
    int (*xfer)(struct spi_device *spi, struct spi_msg *msg);
};

struct spi_device {
    struct device dev;
    int chip_select;
    // TODO: add speed, mode etc...
    uint8_t mode;
};

struct spi_req {
    uint8_t *tx_buf;
    uint8_t *rx_buf;
    unsigned int len;
    uint8_t cs_change;
    struct spi_req *next;
};

struct spi_msg {
    struct spi_device *dev;
    struct spi_req *req;
    void (*complete)(void *priv);
    void  *priv;
    struct spi_msg *next;
};

int spi_sync(struct spi_device *dev, struct spi_msg *msg);
int spi_async(struct spi_device *dev, struct spi_msg *msg);

static inline void spi_msg_init(struct spi_msg *msg)
{
    memset(msg, 0, sizeof *msg);
}

#endif
