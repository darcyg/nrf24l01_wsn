#ifndef __NRF_MODULE_H
#define __NRF_MODULE_H

// ------------------------------------------------------------------
// nrf registers
// ------------------------------------------------------------------

#include "nrfmod.h"

// extern struct spi_control;

// ------------------------------------------------------------------
// GPIO configuration
// ------------------------------------------------------------------

#define NRF24L01_GPIO_IRQ_DESC         "nrf24l01 irq"
#define NRF24L01_GPIO_IRQ_DEVICE_DESC  "nrf24l01 device"

#define NRF24L01_GPIO_CE_DESC          "nrf24l01 ce"

#define GPIO_LOW  0
#define GPIO_HIGH 1

#define SPI_BUS_SPEED 1000000
// #define NRF_TIMEOUT 50 // 50 ms
#define NRF_TIMEOUT 100 // 100 ms

#define NET_TX_BUFFER_SIZE 32

#endif
