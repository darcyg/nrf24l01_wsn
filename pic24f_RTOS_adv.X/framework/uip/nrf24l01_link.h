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

#ifndef NRF24L01_LINK_H
#define NRF24L01_LINK_H

#include "uip/uip.h" // For uip_ipaddr_t type

#define NRF_TIMEOUT           100 /*!< time slot for RX/TX (in ms) */
#define NRF_LINK_BUFFER_SIZE  400 /*!< RX/TX buffer MTU */
#define NRF_LINK_BUFFER_COUNT 3   /*!< RX/TX buffer pool size */

#define NRF_RETRANSMIT_PKG    3   /*!< packet retransmission count */
#define NRF_RETRANSMIT        5   /*!< tcp frame retransmission count before drop */


struct __attribute__((__packed__)) link_data {
    // Link header
    uint8_t pad;
    uint8_t ctl_byte;
    // Raw data transmitted
    uint8_t data[NRF_LINK_BUFFER_SIZE+2]; // uip buffer (+2)
};

struct nrf_buffer;

struct nrf_buffer {
    struct link_data buf;
    unsigned int len;
    struct nrf_buffer *next;
};

void nrf_link_init(const int dev_id, uip_ipaddr_t ip, uip_ipaddr_t gateway, uip_ipaddr_t mask, UBaseType_t priority);

struct nrf_buffer* nrf_link_get_buffer(void);

void nrf_link_free_buffer(struct nrf_buffer *buf);

void nrf_link_send_buffer(struct nrf_buffer *buf);

BaseType_t nrf_link_wait_rx(struct nrf_buffer **buffer, TickType_t timeout);

unsigned int nrf_link_get_rx_packets(void);
unsigned int nrf_link_get_rx_dropped(void);
unsigned int nrf_link_get_tx_packets(void);
unsigned int nrf_link_get_tx_dropped(void);

#endif  /* NRF24L01_LINK_H */
