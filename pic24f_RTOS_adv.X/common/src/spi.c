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

#include "spi.h"
#include "logger.h"
#include "FreeRTOS.h"
#include "task.h"

int spi_async(struct spi_device *dev, struct spi_msg *msg)
{
    msg->dev = dev;
    ((struct spi_driver*)(dev->dev.parent->driver))->xfer(dev, msg);
    return 0;
}

static void spi_complete(void *priv)
{
    portBASE_TYPE xHigherPriorityTaskWoken;
    vTaskNotifyGiveFromISR((TaskHandle_t)priv, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken != pdFALSE) {
        taskYIELD();
    }
}

int spi_sync(struct spi_device *dev, struct spi_msg *msg)
{
    int ret;

    msg->complete = spi_complete;
    msg->priv = xTaskGetCurrentTaskHandle();

    if ((ret = spi_async(dev, msg)) != 0) {
        return ret;
    }
    ulTaskNotifyTake(pdTRUE, (TickType_t)portMAX_DELAY);
    return 0;
}
