/*
 * File:   delay.h
 * Author: Simon
 *
 * Created on 2 décembre 2012, 21:00
 */

#ifndef delay_H
#define delay_H

    #include "HardwareProfile.h"

    // Define your clock speed here ************************************/
    // Sample clock speed for a 16-bit processor
    #define GetSystemClock()            FOSC
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (FCY)

    extern void __delay32(unsigned long);
    #define delay_us(x) __delay32(((x*GetInstructionClock())/1000000L)) // delays x us
    #define delay_ms(x) __delay32(((x*GetInstructionClock())/1000L))  // delays x ms

#endif
