
#include "Compiler.h"
#include "HardwareProfile.h"
#include "blink_LED.h"
#include "timer1.h"
#include "uip/uip.h"

volatile unsigned long blink_timer;
// clock_time_t b_timer = 0;
struct timer b_timer;

void blink_LED_init()
{
// pin config
#ifdef BLINK_LED_PIN
    #if BLINK_LED_PIN == 9
        #warning BLINK_LED_PIN on RB9
         #define BLINK_LED_CONFREGISTER           TRISB
         #define BLINK_LED_IOREGISTER             LATB
         #define BLINK_LED_PINMASK                0x0200 // RB9
    #elif BLINK_LED_PIN == 14
        #warning BLINK_LED_PIN on RB14
        #define BLINK_LED_CONFREGISTER            TRISB
        #define BLINK_LED_IOREGISTER              LATB
        #define BLINK_LED_PINMASK                 0x4000 // RB14
    #elif BLINK_LED_PIN == 11
        #warning BLINK_LED_PIN on RB11
        #define BLINK_LED_CONFREGISTER            TRISB
        #define BLINK_LED_IOREGISTER              LATB
        #define BLINK_LED_PINMASK                 0x0800 // RB11
    #else
        #error BLINK_LED config <BLINK_LED_PIN> out of range
    #endif
#else
    #error BLINK_LED config <BLINK_LED_PIN> not defined
#endif

    // heart blink led config
    set_output(BLINK_LED_CONFREGISTER, BLINK_LED_PINMASK);
    // blink_timer = (set_irq ? 1:0);
    timer_set(&b_timer, TIMER1_TICK_PER_SECOND/blink_LED_TOGGLES_PER_SEC);
    blink_stop_irq();
    blink_LED_OFF();
}

inline void blink_stop_irq()
{
    blink_timer = 0;
}

inline void blink_restart_irq()
{
    blink_timer = 1;
}

inline void blink_LED_ON()
{
    set_bit(BLINK_LED_IOREGISTER, BLINK_LED_PINMASK);
}

inline void blink_LED_OFF()
{
    clr_bit(BLINK_LED_IOREGISTER, BLINK_LED_PINMASK);
}

inline void blink_LED_TOGGLE()
{
    if(BLINK_LED_IOREGISTER & BLINK_LED_PINMASK)
    {
        blink_LED_OFF();
    }
    else
    {
        blink_LED_ON();
    }
}

void blink_periodic( void )
{
    if(timer_expired(&b_timer)) {
        timer_reset(&b_timer);
        blink_LED_TOGGLE();
    }
}

void blink_set_period(clock_time_t period)
{
    b_timer.interval = period;
}

inline void blink_irq_handler()
{
    if(blink_timer != 0)
    {
        blink_timer++;
        if(blink_timer >= (unsigned long)TIMER1_TICK_PER_SECOND/blink_LED_TOGGLES_PER_SEC)
        {
            blink_LED_TOGGLE();
            blink_restart_irq();
        }
    }
}
