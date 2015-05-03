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

#ifndef HARDWARE_PROFILE_H
#define	HARDWARE_PROFILE_H

// Generic includes
#include "p24FJ64GB002.h"
// #include <p24Fxxxx.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include <stdint.h>

// Current firmware version
#define VERSION "v0.01"

//* CLOCK CONFIGURATION *********************************************
#define FOSC                     32000000UL
#define FCY                      (FOSC/2)

#define GetSystemClock()         FOSC
#define GetPeripheralClock()     GetSystemClock()
#define GetInstructionClock()    FCY

//* UART2 CONFIGURATION *********************************************
#define BAUDRATE2         57600UL
#define BRG_DIV2          4
#define BRGH2             1
#define UART2_FIFO_SIZE   64

#define LOG_BUFFER_SIZE   256 // Must be a power of 2
#define LOG_MAX_MSG_SIZE  48

//* nrf24l01 ********************************************************
// TODO: remove this configuration and use in in priv structure in board_config (multiple nrf)
#define NRF24L01_PIN_CE  5
#define NRF24L01_PIN_CSN 101
#define NRF24L01_PIN_IRQ 1 // EXT_IRQ_INT1
#define NRF24L01_INTERRUPT_MODE
#define NRF24L01_PLAYLOAD_SIZE  0 // 0 = dynamic

#define EXT_IRQ_INT1

#endif	/* MAIN_H */
