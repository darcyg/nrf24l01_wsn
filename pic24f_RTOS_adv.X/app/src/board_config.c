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
#include <stdint.h>
#include "device.h"
#include "spi.h"
#include "i2c.h"
// Board drivers
#include "pic24f_gpio.h"
#include "pic24f_adc.h"
#include "pic24f_ext_irq.h"
#include "pic24f_uart2.h"
#include "pic24f_spi2.h"
#include "pic24f_i2c.h"
// Device drivers
#include "ir_control.h"
#include "nrf24l01.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

static void InitClock()
{
    //#error PIC24FJ64GB002 = __PIC24FJ64GB002__
    OSCCON = 0x1102;    // Enable secondary oscillator
    CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
    //OSCCON = 0x0000;	 //8Mhz
    //CLKDIV	=	0x0000;	 //do not divide
    //On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
    //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    //This allows the device to power up at a lower initial operating frequency, which can be
    //advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
    //operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
    //power up the PLL.
    {
        unsigned int pll_startup_counter = 6000;
        CLKDIVbits.PLLEN = 1;
        while (pll_startup_counter--);
    }
}

static void InitPinMuxing()
{
    // Set analog pins to digital
    AD1PCFGbits.PCFG = 0xFFFF;
    // unlock registers
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    // UART2 muxing
    RPOR0bits.RP0R    = 5;  // UART2 TX on PIN RP0  (B0)
    RPINR19bits.U2RXR = 1;  // UART2 RX on PIN RP1  (B1)
    // SPI2 muxing
    RPOR6bits.RP13R   = 11; // SPI2 SCK on PIN RP13 (B13)
    RPOR7bits.RP15R   = 10; // SPI2 SDO on PIN RP15 (B15)
    RPINR22bits.SDI2R = 14; // SPI2 SDI on PIN RP14 (B14)
    // EXT_IRQ muxing
    RPINR0bits.INT1R  = 5;  // EXT INT1 on PIN RP5  (A0)
    // IR_LED muxing
    RPOR3bits.RP7R    = 18; // IR  OC1  on PIN RP7  (B7)
    // lock registers
    __builtin_write_OSCCONL(OSCCON | 0x40);
}

struct spi_device spi2_devices[] = {
    {
        .dev.name    = "nrf0",
        .dev.driver  = &nrf24l01_driver,
        .chip_select = GPIO_PORTA | 1,   // Chip select on RA1
        .dev.priv = &(struct nrf24l01_priv) {
            .pin_ce  = GPIO_PORTB | 5,
            .pin_irq = INT1_PIN
        }
    }
};

struct device platform_devices[] = {
    {
        .name = "gpio",
        .driver = &pic24f_gpio_driver
    }, {
        .name = "adc",
        .driver = &pic24f_adc_driver
    }, {
        .name = "ext_irq",
        .driver = &pic24f_ext_irq_driver
    }, {
        .name = "ir",
        .driver = &pic24f_ir_ctl_driver,
        .priv = &(struct pic24f_ir_ctl_priv) {
            .pin_led = GPIO_PORTB | 7
        }
    }
};

struct bus platform_bus_devices[] = {
    {
        .name = "spi2",
        .driver = (struct bus_driver*)&pic24f_spi2_driver,
        BUS_LINK_DEVICES(spi2_devices),
        BUS_LINK_BUSES_NULL
    },
    {
        .name = "i2c2",
        .driver = (struct bus_driver*)&pic24f_i2c2_driver,
        BUS_LINK_DEVICES_NULL,
        BUS_LINK_BUSES_NULL
    }
};

void setup_device(void)
{
    // Init PLL clock
    InitClock();
    // Init pin muxing
    InitPinMuxing();
    // Init logger first
    uart2_init();

    // Check RAM flag
    if (*((volatile uint16_t*)0x2000) == 0xdead) {
        uart2_print("Warm reset detected\n\r");
    } else {
        uart2_print("Cold reset\n\r");
        *((volatile uint16_t*)0x2000) = 0xdead;
    }

    // Init platform devices
    INIT_PLATFORM_DEVICES(platform_devices, platform_bus_devices);
}
