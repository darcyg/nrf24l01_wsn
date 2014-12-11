/*
 * File:   main.h
 * Author: Simon Piriou <spiriou31@gmail.com>
 *
 * Created on 9 décembre 2012, 01:02
 */

#ifndef MAIN_H
#define MAIN_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "HardwareProfile.h"

    // Current firmware version
    #define VERSION "v1.02"

    #define IsNum(c)            (('0' <= c) && (c <= '9'))
    #define UpperCase(c)        (('a'<= c) && (c <= 'z') ? c - 0x20 : c)

    inline void InitIO();

    int hello_world_appcall(void);
    void uip_appcall(void);

    _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
    _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
    _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
    _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

    inline void InitClock()
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
            while(pll_startup_counter--);
        }
    }

#endif // MAIN_H
