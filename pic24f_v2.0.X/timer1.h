/* 
 * File:   timer1.h
 * Author: mickey
 *
 * Created on November 6, 2014, 5:52 PM
 */

#ifndef TIMER1_H
#define	TIMER1_H

#ifdef	__cplusplus
extern "C" {
#endif

// Used for clock_time_t type
#include "HardwareProfile.h"

// Get timer1 tick count
inline clock_time_t timer1_get_tick(void);

// Init timer1
void timer1_init(void);

//  Timer1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T1Interrupt(void);

// Timer1 interrupt handling function
inline void timer1_irq_handler(void);



#ifdef	__cplusplus
}
#endif

#endif	/* TIMER1_H */

