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

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <stdint.h>
#include <stdbool.h>
#include "device.h"
#include "nrf24l01_defs.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "spi.h"

extern struct driver nrf24l01_driver;

struct nrf24l01_priv {
    uint8_t pin_ce;
    uint8_t pin_irq;
    // Driver implementation specific fields
    struct spi_req req_head;
    struct spi_req req_data;
    struct spi_msg msg;
    uint8_t rx_buf[32];
    uint8_t tx_buf[32];
    uint8_t reg_status;
    uint8_t reg_config;
    uint8_t rx_addr_p1[5];
};

void nrf24l01_set_configuration(struct device *dev,
                                void (*irq_callback)(void*),
                                void *irq_priv,
                                unsigned char config,
                                unsigned char en_aa,
                                unsigned char en_rxaddr,
                                unsigned char setup_aw,
                                unsigned char setup_retr,
                                unsigned char rf_ch,
                                unsigned char rf_setup,
                                unsigned char * rx_addr_p0,
                                unsigned char * rx_addr_p1,
                                unsigned char rx_addr_p2,
                                unsigned char rx_addr_p3,
                                unsigned char rx_addr_p4,
                                unsigned char rx_addr_p5,
                                unsigned char * tx_addr,
                                unsigned char rx_pw_p0,
                                unsigned char rx_pw_p1,
                                unsigned char rx_pw_p2,
                                unsigned char rx_pw_p3,
                                unsigned char rx_pw_p4,
                                unsigned char rx_pw_p5,
                                unsigned char dynpd,
                                unsigned char feature);

// power-up, power-down functions
void nrf24l01_power_up(struct device *dev);
void nrf24l01_power_down(struct device *dev);
void nrf24l01_clear_ce(struct device *dev);
void nrf24l01_set_ce(struct device *dev);

// device settings
void nrf24l01_set_as_rx(struct device *dev);
void nrf24l01_set_as_tx(struct device *dev);
uint8_t nrf24l01_get_config(struct device *dev);
uint8_t nrf24l01_get_status(struct device *dev);
uint8_t nrf24l01_get_fifo_status(struct device *dev);

// IRQ management
uint8_t nrf24l01_irq_clear_all(struct device *dev);
// TODO: remove
int nrf24l01_irq_pin_active();

// TX/RX buffers management
uint8_t nrf24l01_flush_tx(struct device *dev);
uint8_t nrf24l01_flush_rx(struct device *dev);
uint8_t nrf24l01_write_tx_payload(struct device *dev, uint8_t* data, unsigned int len);
uint8_t nrf24l01_write_tx_payload_no_ack(struct device *dev, uint8_t* data, unsigned int len);
uint8_t nrf24l01_reuse_tx_pl(struct device *dev);

uint8_t nrf24l01_r_rx_pl_wid(struct device *dev, uint8_t* data);
uint8_t nrf24l01_read_rx_payload(struct device *dev, uint8_t* data, unsigned int len);
uint8_t nrf24l01_activate_feature(struct device *dev);

// Start a new TX transfer
void nrf24l01_transmit(struct device *dev);

uint8_t *nrf24l01_get_rx_addr(struct device *dev);
uint8_t nrf24l01_set_tx_addr(struct device *dev, uint8_t *addr, bool is_ack);

#endif /*NRF24L01_H_*/
