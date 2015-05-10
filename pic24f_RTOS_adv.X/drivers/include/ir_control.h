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

#ifndef IR_CONTROL_H
#define	IR_CONTROL_H

extern struct driver pic24f_ir_ctl_driver;

void ir_sendRC5(uint32_t data, unsigned int nbits);
void ir_sendCanal(uint32_t data, unsigned int nbits);
void ir_sendNEC(uint32_t data, unsigned int nbits);

// TODO: move implementation specific data somewhere else
#include "timers.h"

#define IR_QUEUE_SIZE  16

struct ir_cmd;
typedef void (*ir_handler) (struct ir_cmd *cmd);

struct ir_cmd {
    uint32_t     data;    /*!< Data to send */
    unsigned int nbits;   /*!< Number of bits to send */
    unsigned int step;    /*!< State for IR handle function */
    ir_handler   handler; /*!< Function handler to send data */
};

struct pic24f_ir_ctl_priv {
    uint8_t pin_led;                           /*!< gpio IR LED pin */
    // Internal driver variables
    struct ir_cmd ir_cmd_queue[IR_QUEUE_SIZE]; /*!< Circular buffer to handle IR commands */
    volatile struct ir_cmd *cur_ir_cmd;        /*!< Current processed command */
    volatile unsigned int ir_head;             /*!< IR command queue head index */
    volatile unsigned int ir_tail;             /*!< IR command queue tail index */
    TimerHandle_t ir_timer;                    /*!< Timer for IR timeout */
};

#endif	/* IR_CONTROL_H */
