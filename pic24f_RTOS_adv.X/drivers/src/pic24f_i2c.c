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

#include "FreeRTOS.h"
#include "pic24f_i2c.h"
#include "logger.h"

static int pic24f_i2c2_init(struct bus *dev);
static int pic24f_i2c_init_dev(struct bus *bus, struct device *dev);
static int pic24f_i2c_transfer(struct i2c_device *i2c, struct i2c_msg *msg);

struct i2c_driver pic24f_i2c2_driver = {
    .driver         = {
        .init       = pic24f_i2c2_init,
        .init_dev   = pic24f_i2c_init_dev
    },
    .xfer           = pic24f_i2c_transfer
};

static int pic24f_i2c2_init(struct bus *dev)
{
    I2C2CON = 0x0200; // Disable slew rate control
    I2C2BRG = 37; // 400 kHz

    // init I2C2 module
    IEC3bits.MI2C2IE = 0;
    I2C2CONbits.I2CEN = 1;
    // Initiate stop bit on bus
    I2C2CONbits.PEN = 1;
    // Wait for hardware to reset bus
    while (I2C2CONbits.PEN);
    I2C2CONbits.I2CEN = 0;

    // Enable i2c2 interrupts in master mode
    IFS3bits.MI2C2IF = 0;   // Clear Interrupt
    IEC3bits.MI2C2IE = 1;   // Enable interrupt
    IPC12bits.MI2C2IP = configKERNEL_INTERRUPT_PRIORITY;

    log_debug("i2c (%x) device %d", SPI2STAT, dev->id);
    return 0;
}

static int pic24f_i2c_init_dev(struct bus *bus, struct device *dev)
{
    log_debug("init i2c dev %d (%d)", dev->id, bus->id);
    return 0;
}

static int pic24f_i2c_transfer(struct i2c_device *i2c, struct i2c_msg *msg)
{
    log_info("xfer i2c dev %d", i2c->dev.id);
    return 0;
}
void __attribute__((interrupt,no_auto_psv)) _MI2C2Interrupt(void)
{
    IFS3bits.MI2C2IF = 0; // Clear irq flag
    I2C2CONbits.I2CEN = 0; // Disable i2c master
    log_info("i2c irq");
}

//function iniates a start condition on bus
inline static void i2c2_start(void)
{
    I2C2CONbits.ACKDT = 0;  //Reset any previous Ack
    I2C2CONbits.SEN = 1;    //Initiate Start condition
}
