#include <xc.h>
#include <leds.h>
#include <stdbool.h>

#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0

void LED_On ( LED led )
{
    LATBbits.LATB4 = LED_ON;
}

void LED_Off ( LED led )
{
    LATBbits.LATB4 = LED_OFF;
}

void LED_Toggle ( LED led )
{
    LATBbits.LATB4 ^= 1;
}

bool LED_Get ( LED led )
{
    return ( ( LATBbits.LATB4 == LED_ON ) ? true : false ) ;
}

void LED_Enable ( LED led )
{
    TRISBbits.TRISB4 = OUTPUT;
}

