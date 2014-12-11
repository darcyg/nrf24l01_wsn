/* 
 * File:   blink_LED.h
 * Author: simon
 *
 * Created on January 6, 2013, 12:45 AM
 */

#ifndef BLINK_LED_H
#define	BLINK_LED_H

#ifdef	__cplusplus
extern "C" {
#endif

    void blink_LED_init( void );
    inline void blink_LED_ON( void );
    inline void blink_LED_OFF( void );
    inline void blink_LED_TOGGLE( void );

    void blink_periodic( void );

    inline void blink_irq_handler( void );
    inline void blink_stop_irq( void );
    inline void blink_restart_irq( void );

#ifdef	__cplusplus
}
#endif

#endif	/* BLINK_LED_H */

