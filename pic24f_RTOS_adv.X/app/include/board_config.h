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

#ifndef BOARD_CONFIG_H
#define	BOARD_CONFIG_H

// Generic includes
#include <p24Fxxxx.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include <stdint.h>

// Useful macros
#define likely(x)         __builtin_expect((x),1)
#define unlikely(x)       __builtin_expect((x),0)
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*array))


// Current firmware version
#define VERSION "v0.01"

/***************************************
 *        clock configuration          *
 ***************************************/
#define FOSC                     32000000UL
#define FCY                      (FOSC/2)

#define GetSystemClock()         FOSC
#define GetPeripheralClock()     GetSystemClock()
#define GetInstructionClock()    FCY

/***************************************
 *        uart2 configuration          *
 ***************************************/
#define BAUDRATE2         57600UL
#define BRG_DIV2          4
#define BRGH2             1
#define UART2_FIFO_SIZE   16

#define LOG_BUFFER_SIZE   256 // Must be a power of 2
#define LOG_MAX_MSG_SIZE  40

/***************************************
 *      compilation configuration      *
 ***************************************/
// TODO: Add config compilation switches for all modules
#define EXT_IRQ_INT1

/***************************************
 *      application configuration      *
 ***************************************/
#define nrftask_PRIORITY    (tskIDLE_PRIORITY + 3)
#define maintask_PRIORITY   (tskIDLE_PRIORITY + 2)
#define rxtask_PRIORITY     (tskIDLE_PRIORITY + 1)

#define BLINK_LED_PIN (GPIO_PORTB | 4)
#define TST_BTN_PIN   (GPIO_PORTA | 4)

#define TEMP_SENSOR_PIN 4
#define LUM_SENSOR_PIN  5

#define CONFIGURE_IP(ip, addr) uip_ipaddr(ip, addr)
#define IP_ADDR  192, 168,  20, 4
#define GW_ADDR  192, 168,  20, 1
#define NET_MASK 255, 255, 255, 0

#define MQTT_NODE_ID "home01"
#define MQTT_SRV 192, 168,  20, 1

#endif	/* BOARD_CONFIG_H */
