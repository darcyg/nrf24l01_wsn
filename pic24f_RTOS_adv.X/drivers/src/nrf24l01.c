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

#include "nrf24l01.h"
#include "pic24f_gpio.h"
#include "pic24f_ext_irq.h"
#include "logger.h"
#include "delay.h"

static int nrf24l01_init(struct device *dev);
static inline uint8_t nrf24l01_write_register(struct device *dev, uint8_t reg, uint8_t* data, unsigned int len);
static inline uint8_t nrf24l01_read_register(struct device *dev, uint8_t reg, uint8_t* data, unsigned int len);
static uint8_t nrf24l01_execute_command(struct device *dev, uint8_t cmd, uint8_t* data, unsigned int len, uint8_t copydata);

struct driver nrf24l01_driver = {
    .init   = nrf24l01_init
};

static int nrf24l01_init(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    log_info("init nrf %s", dev->name);
    // Configure chip enable pin as output
    gpio_configure(nrf_dev->pin_ce, 0, 0);

    nrf_dev->msg.req = &nrf_dev->req_head;
    nrf_dev->req_data.next = NULL;
    nrf_dev->req_head.rx_buf = &nrf_dev->reg_status;
    nrf_dev->req_head.cs_change = 0;
    nrf_dev->req_head.len = 1;

    nrf_dev->reg_status = 0x00;
    return 0;
}

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
                                unsigned char feature)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    nrf_dev->reg_config = config;
    uint8_t data[5];

    // Register external interrupt routine
    log_info("irq reg %d", register_ext_irq(nrf_dev->pin_irq, FALLING_EDGE,
                           irq_callback, irq_priv,
                           configKERNEL_INTERRUPT_PRIORITY));

    nrf24l01_write_register(dev, nrf24l01_EN_AA, &en_aa, 1);
    nrf24l01_write_register(dev, nrf24l01_EN_RXADDR, &en_rxaddr, 1);
    nrf24l01_write_register(dev, nrf24l01_SETUP_AW, &setup_aw, 1);
    nrf24l01_write_register(dev, nrf24l01_SETUP_RETR, &setup_retr, 1);
    nrf24l01_write_register(dev, nrf24l01_RF_CH, &rf_ch, 1);
    nrf24l01_write_register(dev, nrf24l01_RF_SETUP, &rf_setup, 1);

    if (rx_addr_p0 != NULL)
        nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P0, rx_addr_p0, 5);
    else{
        data[0] = nrf24l01_RX_ADDR_P0_B0_DEFAULT_VAL;
        data[1] = nrf24l01_RX_ADDR_P0_B1_DEFAULT_VAL;
        data[2] = nrf24l01_RX_ADDR_P0_B2_DEFAULT_VAL;
        data[3] = nrf24l01_RX_ADDR_P0_B3_DEFAULT_VAL;
        data[4] = nrf24l01_RX_ADDR_P0_B4_DEFAULT_VAL;

        nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P0, data, 5);
    }

    if (rx_addr_p1 != NULL)
        memcpy(nrf_dev->rx_addr_p1, rx_addr_p1, 5);
    else{
        nrf_dev->rx_addr_p1[0] = nrf24l01_RX_ADDR_P1_B0_DEFAULT_VAL;
        nrf_dev->rx_addr_p1[1] = nrf24l01_RX_ADDR_P1_B1_DEFAULT_VAL;
        nrf_dev->rx_addr_p1[2] = nrf24l01_RX_ADDR_P1_B2_DEFAULT_VAL;
        nrf_dev->rx_addr_p1[3] = nrf24l01_RX_ADDR_P1_B3_DEFAULT_VAL;
        nrf_dev->rx_addr_p1[4] = nrf24l01_RX_ADDR_P1_B4_DEFAULT_VAL;
    }
    nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P1, nrf_dev->rx_addr_p1, 5);

    nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P2, &rx_addr_p2, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P3, &rx_addr_p3, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P4, &rx_addr_p4, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P5, &rx_addr_p5, 1);

    if (tx_addr != NULL)
        nrf24l01_write_register(dev, nrf24l01_TX_ADDR, tx_addr, 5);
    else{
        data[0] = nrf24l01_TX_ADDR_B0_DEFAULT_VAL;
        data[1] = nrf24l01_TX_ADDR_B1_DEFAULT_VAL;
        data[2] = nrf24l01_TX_ADDR_B2_DEFAULT_VAL;
        data[3] = nrf24l01_TX_ADDR_B3_DEFAULT_VAL;
        data[4] = nrf24l01_TX_ADDR_B4_DEFAULT_VAL;

        nrf24l01_write_register(dev, nrf24l01_TX_ADDR, data, 5);
    }

    nrf24l01_write_register(dev, nrf24l01_RX_PW_P0, &rx_pw_p0, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_PW_P1, &rx_pw_p1, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_PW_P2, &rx_pw_p2, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_PW_P3, &rx_pw_p3, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_PW_P4, &rx_pw_p4, 1);
    nrf24l01_write_register(dev, nrf24l01_RX_PW_P5, &rx_pw_p5, 1);
    nrf24l01_write_register(dev, nrf24l01_DYNPD, &dynpd, 1);
    nrf24l01_write_register(dev, nrf24l01_FEATURE, &feature, 1);
    nrf24l01_write_register(dev, nrf24l01_CONFIG, &config, 1);

    log_info("nrf init done");
}

uint8_t nrf24l01_get_config(struct device *dev)
{
    uint8_t data;
    nrf24l01_read_register(dev, nrf24l01_CONFIG, &data, 1);
    return data;
}

uint8_t nrf24l01_get_status(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    return nrf_dev->reg_status;
}

uint8_t* nrf24l01_get_rx_addr(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    return nrf_dev->rx_addr_p1;
}

void nrf24l01_set_ce(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    gpio_set(nrf_dev->pin_ce, 1);
}

void nrf24l01_clear_ce(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    gpio_set(nrf_dev->pin_ce, 0);
}

uint8_t nrf24l01_flush_tx(struct device *dev)
{
    return nrf24l01_execute_command(dev, nrf24l01_FLUSH_TX, NULL, 0, 1);
}

uint8_t nrf24l01_flush_rx(struct device *dev)
{
    return nrf24l01_execute_command(dev, nrf24l01_FLUSH_RX, NULL, 0, 1);
}

uint8_t nrf24l01_irq_clear_all(struct device *dev)
{
    uint8_t data = nrf24l01_STATUS_RX_DR | nrf24l01_STATUS_TX_DS | nrf24l01_STATUS_MAX_RT;
    return nrf24l01_write_register(dev, nrf24l01_STATUS, &data, 1);
}

static inline uint8_t nrf24l01_write_register(struct device *dev, uint8_t reg, uint8_t* data, unsigned int len)
{
    return nrf24l01_execute_command(dev, nrf24l01_W_REGISTER | (reg & nrf24l01_W_REGISTER_DATA), data, len, 0);
}

static inline uint8_t nrf24l01_read_register(struct device *dev, uint8_t reg, uint8_t* data, unsigned int len)
{
    return nrf24l01_execute_command(dev, reg & nrf24l01_R_REGISTER_DATA, data, len, 1);
}

static uint8_t nrf24l01_execute_command(struct device *dev, uint8_t cmd, uint8_t* data, unsigned int len, uint8_t copydata)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);

    // Set data buffers
    nrf_dev->req_head.tx_buf = &cmd;

    if (len) {
        nrf_dev->req_head.next = &nrf_dev->req_data;
        nrf_dev->req_data.tx_buf = data;
        nrf_dev->req_data.len = len;
        nrf_dev->req_data.rx_buf = copydata ? data : NULL;
    } else {
        nrf_dev->req_head.next = NULL;
    }

    return spi_sync((struct spi_device*)dev, &nrf_dev->msg);
}

void nrf24l01_set_as_rx(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    nrf_dev->reg_config = nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP | nrf24l01_CONFIG_PRIM_RX;
    nrf24l01_write_register(dev, nrf24l01_CONFIG, &nrf_dev->reg_config, 1);
}

void nrf24l01_set_as_tx(struct device *dev)
{
    struct nrf24l01_priv *nrf_dev = (struct nrf24l01_priv*)(dev->priv);
    nrf_dev->reg_config = nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP;
    nrf24l01_write_register(dev, nrf24l01_CONFIG, &nrf_dev->reg_config, 1);

}

uint8_t nrf24l01_r_rx_pl_wid(struct device *dev, uint8_t* data)
{
    return nrf24l01_execute_command(dev, nrf24l01_R_RX_PL_WTD, data, 1, 1);
}

uint8_t nrf24l01_read_rx_payload(struct device *dev, uint8_t* data, unsigned int len)
{
    return nrf24l01_execute_command(dev, nrf24l01_R_RX_PAYLOAD, data, len, 1);
}

uint8_t nrf24l01_activate_feature(struct device *dev)
{
    uint8_t data = 0x73;
    return nrf24l01_execute_command(dev, nrf24l01_ACTIVATE, &data, 1, 0);
}

uint8_t nrf24l01_get_fifo_status(struct device *dev)
{
    uint8_t data = 0;
    nrf24l01_read_register(dev, nrf24l01_FIFO_STATUS, &data, 1);
    return data;
}

void nrf24l01_transmit(struct device *dev)
{
    nrf24l01_set_ce(dev);
    delay_us(20);
    nrf24l01_clear_ce(dev);
}

uint8_t nrf24l01_set_tx_addr(struct device *dev, uint8_t *addr, bool is_ack)
{
    if (is_ack) {
        // TODO: check ret value
        nrf24l01_write_register(dev, nrf24l01_RX_ADDR_P0, addr, 5);
    }
    return nrf24l01_write_register(dev, nrf24l01_TX_ADDR, addr, 5);
}

uint8_t nrf24l01_write_tx_payload_no_ack(struct device *dev, uint8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(dev, nrf24l01_W_TX_PAYLOAD_NO_ACK, data, len, 0);
}

uint8_t nrf24l01_write_tx_payload(struct device *dev, uint8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(dev, nrf24l01_W_TX_PAYLOAD, data, len, 0);
}

uint8_t nrf24l01_reuse_tx_pl(struct device *dev)
{
	return nrf24l01_execute_command(dev, nrf24l01_REUSE_TX_PL, NULL, 0, 0);
}
