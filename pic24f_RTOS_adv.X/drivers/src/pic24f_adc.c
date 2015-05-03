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

#include "HardwareProfile.h"
#include <stdbool.h>
#include "pic24f_adc.h"
#include "device.h"

static int adc_init(struct device *dev);

struct driver pic24f_adc_driver = {
    .init = adc_init
};

static int adc_init(struct device *dev)
{
    AD1CON1bits.SSRC  = 0b111; // SSRC<3:0> = 111 implies internal
                               // counter ends sampling and starts
                               // converting.
    AD1CON3 = 0x1F02;          // Sample time = 31Tad,
                               // Tad = 2 Tcy
    return 0;
}

void adc_configure(uint8_t pin, bool is_adc)
{
    if (is_adc) {
        AD1PCFG &= ~(1<<pin);
    } else {
        AD1PCFG |= (1<<pin);
    }
}

unsigned int adc_get_value(uint8_t pin)
{
    unsigned int value = 0;
    unsigned int i;

    AD1CHS = pin;                   // select ADC channel
    AD1CON1bits.ADON =1;            // turn ADC on
    for(i=0; i<8; i++)
    {
        AD1CON1bits.DONE=0;         //resets DONE bit
        AD1CON1bits.SAMP=1;         //start sample
        while(AD1CON1bits.DONE==0); //wait for conversion to finish
        value += ADC1BUF0;          //get the voltage measurement
    }
    AD1CON1bits.ADON = 0;           // turn ADC off
    return value >> 3;
}
