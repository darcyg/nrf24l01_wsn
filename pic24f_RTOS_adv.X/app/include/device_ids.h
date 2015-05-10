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

#ifndef DEVICE_IDS_H
#define DEVICE_IDS_H

#define DEV_ROOT      0
#define DEV_GPIO      1
#define DEV_ADC       2
#define DEV_EXT_IRQ   3
#define DEV_IR_0      4

#define DEV_SPI2      0x10
#define DEV_NRF24L01_0 (DEV_SPI2 | 1)

#define DEV_I2C2      0x20

#endif
