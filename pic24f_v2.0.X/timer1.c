#include "HardwareProfile.h"
#include "timer1.h"

unsigned long int timer1_tick;

inline clock_time_t timer1_get_tick(void)
{
    return timer1_tick;
}

inline void timer1_irq_handler(void)
{
    timer1_tick++;
    IFS0bits.T1IF = 0;
}

void timer1_init()
{
    // timer1 config
    PR1 = TIMER1_PERIOD;
    TMR1 = 0;
    IPC0bits.T1IP = 5;   //set interrupt priority
    T1CON = 0b1000000000000000; //turn on the timer
    IFS0bits.T1IF = 0;   //reset interrupt flag
    IEC0bits.T1IE =
#ifdef TIMER1_INTERRUPT_MODE
        1;
#else
        0;
#endif

    T1CON = 0x8000
#ifdef TIMER1_USE_EXTERNAL
        | 0x0002
#endif
#if TIMER1_PRESCALE == 1
        | 0x0000
#elif TIMER1_PRESCALE == 8
        | 0x0010
#elif TIMER1_PRESCALE == 64
        | 0x0020
#elif TIMER1_PRESCALE == 256
        | 0x0030
#else
#error TIMER1 config <TIMER1_PRESCALE> not defined
#endif
        ;
    timer1_tick = 0;
}

