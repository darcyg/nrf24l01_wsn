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
#include "FreeRTOS.h"
#include "spi.h"
#include "pic24f_gpio.h"
#include "pic24f_spi2.h"
#include "logger.h"

#include <stdbool.h>
#include <errno.h>

static int pic24f_spi2_init(struct bus *dev);
static int pic24f_spi_init_dev(struct bus *bus, struct device *dev);
static int pic24f_spi_transfer(struct spi_device *spi, struct spi_msg *msg);

struct spi_driver pic24f_spi2_driver = {
    .driver       = {
        .init     = pic24f_spi2_init,
        .init_dev = pic24f_spi_init_dev
    },
    .xfer         = pic24f_spi_transfer
};

#define SPI_SPRE_1 0x001C
#define SPI_SPRE_2 0x0018
#define SPI_SPRE_3 0x0014
#define SPI_SPRE_4 0x0010
#define SPI_SPRE_5 0x000C
#define SPI_SPRE_6 0x0008
#define SPI_SPRE_7 0x0004
#define SPI_SPRE_8 0x0000

#define SPI_PPRE_1  0x0003
#define SPI_PPRE_4  0x0002
#define SPI_PPRE_16 0x0001
#define SPI_PPRE_64 0x0000

static volatile struct pic24f_spi_info *spi2_master_info = NULL;

static int pic24f_spi2_init(struct bus *dev)
{
    // Init queue ptrs
    ((struct pic24f_spi_info*)dev->priv)->head = NULL;
    ((struct pic24f_spi_info*)dev->priv)->tail = NULL;

    // Device already init
    if (spi2_master_info) {
        return -EINVAL;
    }
    // Link this device to interrupt handler
    spi2_master_info = dev->priv;

    SPI2STATbits.SPIEN = 0; // disable spi2 module

    // init SPI module
    IFS2bits.SPI2IF = 0;
    IEC2bits.SPI2IE = 1; // Enable interrupt
    IPC8bits.SPI2IP = configKERNEL_INTERRUPT_PRIORITY;

    // init SPI default : Fsck=16M/16/1 = 1M
    // CKE = 1: Change SDO on CLK transition from low to high
    // CKP = 0: CLK active on high level
    SPI2CON1 = 0x0120 | ((SPI_SPRE_1 | SPI_PPRE_16) & 0x001F);
    // SPI2CON2 = 0x0001; // spi enhanced mode enabled, framed spi disabled
    SPI2CON2 = 0x0001;          // spi enhanced mode enabled, framed spi disabled

    SPI2STATbits.SISEL = 0b101; // Interrupt when TX FIFO is empty
    SPI2STATbits.SPIROV = 0;    // Clear overflow flag

    log_debug("spi (%x) device %d", SPI2STAT, dev->id);
    return 0;
}

static int pic24f_spi_init_dev(struct bus *bus, struct device *dev)
{
    struct spi_device *spi_dev = (struct spi_device*)dev;

    // Init chip select pin
    gpio_set(spi_dev->chip_select, !(spi_dev->mode & SPI_CS_HIGH));
    gpio_configure(spi_dev->chip_select, 0, 0);
    log_debug("spi init <%d> (%d)", dev->id, bus->id);
    return 0;
}

static int pic24f_spi_exec_request(struct pic24f_spi_info *spi_info)
{
    unsigned int i;

    // TODO: check spi module settings
    // Check rx fifo is empty
    if (!SPI2STATbits.SRXMPT) {
        log_error("spi2 rx fifo not empty");
        return -EDATA;
    }

    // Set chip select
    gpio_set(spi_info->head->dev->chip_select, !!(spi_info->head->dev->mode & SPI_CS_HIGH));
    // Write message data
    spi_info->tx_index = 0;
    spi_info->rx_index = 0;
    // log_debug("req %d %x", spi_info->req->len, SPI2STAT);

    for (i = 0; (i < 8) && (spi_info->tx_index < spi_info->req->len); i++) {
        SPI2BUF = spi_info->req->tx_buf[spi_info->tx_index++];
    }
    return 0;
}

static int pic24f_spi_transfer(struct spi_device *spi, struct spi_msg *msg)
{
    int ret;
    struct pic24f_spi_info* spi_info = (struct pic24f_spi_info*)spi->dev.parent->priv;

    // if (!spi_info->req) {
    //     // Error: no request
    //     return -EINVAL;
    // }

    // Lock IRQ
    portENTER_CRITICAL();
    if (spi_info->head) {
        // Transfer on going, add message to queue
        spi_info->tail->next = msg;
        spi_info->tail = msg;
        portEXIT_CRITICAL();
        return 0;
    }
    // No requests on going
    spi_info->head = msg;
    spi_info->tail = msg;
    portEXIT_CRITICAL();

    spi_info->req = msg->req;
    // Configure SPI module according to device
    SPI2CON1 = 0x0020 | (!(msg->dev->mode & SPI_CPHA) << 8) | ((msg->dev->mode & SPI_CPOL) << 5) | ((SPI_SPRE_1 | SPI_PPRE_16) & 0x001F);
    log_debug("config %x", SPI2CON1);
    // Enable SPI module
    SPI2STATbits.SPIEN = 1;
    // Start new transfer
    ret = pic24f_spi_exec_request(spi_info);
    return ret;
}

void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void)
{
    IFS2bits.SPI2IF = 0;
    // Save RX bytes
    while (SPI2STATbits.SRXMPT);
    if (spi2_master_info->req->rx_buf != NULL) {
        while (!SPI2STATbits.SRXMPT) {
            spi2_master_info->req->rx_buf[spi2_master_info->rx_index++] = SPI2BUF;
        }
    } else {
        uint8_t c;
        while (!SPI2STATbits.SRXMPT) {
            spi2_master_info->rx_index++;
            c = SPI2BUF;
        }
    }

    // Send remaning bytes
    if (spi2_master_info->tx_index < spi2_master_info->req->len) {
        // Wait for shift register to be empty
        unsigned int i;
        for (i = 0; (i < 8) && (spi2_master_info->tx_index < spi2_master_info->req->len); i++) {
            SPI2BUF = spi2_master_info->req->tx_buf[spi2_master_info->tx_index++];
        }
        return;
    }

    // Request transfer done, check errors
    if (spi2_master_info->rx_index != spi2_master_info->tx_index) {
        log_error("IRQ error %d %d (%x)", spi2_master_info->rx_index, spi2_master_info->tx_index, SPI2STAT);
    }

    // Start new spi request transfer
    if (spi2_master_info->req->next != NULL) {
        int ret;
        if (spi2_master_info->req->cs_change) {
            // Clear chip select
            gpio_set(spi2_master_info->head->dev->chip_select, !(spi2_master_info->head->dev->mode & SPI_CS_HIGH));
        }
        spi2_master_info->req = spi2_master_info->req->next;
        if ((ret = pic24f_spi_exec_request((struct pic24f_spi_info*)spi2_master_info)) != 0) {
            log_error("FAIL IRQ");
        }
        return;
    }
    // Clear chip select
    gpio_set(spi2_master_info->head->dev->chip_select, !(spi2_master_info->head->dev->mode & SPI_CS_HIGH));

    struct spi_msg *cur_msg = spi2_master_info->head;
    // Check if there is another message on queue
    if (spi2_master_info->head != spi2_master_info->tail) {
        // Message on queue, send it
        int ret;
        uint16_t spi_conf;
        spi2_master_info->head = spi2_master_info->head->next;
        spi2_master_info->req = spi2_master_info->head->req;

        // Configure SPI module according to device
        spi_conf = 0x0020 | (!(spi2_master_info->head->dev->mode & SPI_CPHA) << 8) | ((spi2_master_info->head->dev->mode & SPI_CPOL) << 5) | ((SPI_SPRE_1 | SPI_PPRE_16) & 0x001F);
        if (SPI2CON1 != spi_conf) {
            // Disable SPI module
            SPI2STATbits.SPIEN = 0;
            SPI2CON1 = spi_conf;
            // Enable SPI module
            SPI2STATbits.SPIEN = 1;
        }

        if ((ret = pic24f_spi_exec_request((struct pic24f_spi_info*)spi2_master_info)) != 0) {
            log_error("FAIL IRQ 2 %d", ret);
        }
    } else {
        // Request transfer done, disable SPI module
        SPI2STATbits.SPIEN = 0;
        spi2_master_info->head = NULL;
    }
    // Call user callback
    cur_msg->complete(cur_msg->priv);
}
