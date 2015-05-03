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

#include "serial/serial.h"
// os modules
#include "FreeRTOS.h"
#include "task.h"
// Utils
#include "logger.h"
// I/O modules
#include "pic24f_uart2.h"

static void serial_task(void *arg)
{
    char c;

    while (1) {
        if (uart2_getc(&c, portMAX_DELAY)) {
            // Char received, process
            log_info("rx char %c", c);
        }
    }
}

void serial_init(UBaseType_t priority)
{
    // Create serial task
    xTaskCreate(serial_task, "rx", configMINIMAL_STACK_SIZE, NULL, priority, NULL);
}
