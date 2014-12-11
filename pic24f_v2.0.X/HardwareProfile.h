/* 
 * File:   HardwareProfile.h
 * Author: Simon
 *
 * Created on 21 octobre 2012, 21:52
 */

#ifndef HARDWARE_PROFILE_H
#define	HARDWARE_PROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    //* INCLUDE FILES ***************************************************
    // #include "p24FJ64GB002.h"
    #include <p24Fxxxx.h>
    #include "Compiler.h"
    #include "GenericTypeDefs.h"
    
    //* TYPEDEF *********************************************************
    #ifndef bool
    #define bool unsigned char
    #endif
    #ifndef false
    #define false 0
    #endif
    #ifndef true
    #define true !false
    #endif

    //* TRIS ************************************************************
    #define INPUT_PIN           1
    #define OUTPUT_PIN          !INPUT_PIN

    #define set_bit(r,m) r = r | m
    #define clr_bit(r,m) r = r & (~m)

    #if INPUT_PIN == 1
        #define set_output(r,m) clr_bit(r,m)
        #define set_input(r,m) set_bit(r,m)
    #else
        #define set_output(r,m) set_bit(r,m)
        #define set_input(r,m) clr_bit(r,m)
    #endif

    //* CLOCK CONFIGURATION *********************************************
    #define FOSC    32000000UL
    #define FCY     (FOSC/2)

    //* TIMER1 CONFIGURATION ********************************************
    // This is the main timer for common stuffs (LED, PWM, delays ...)
    #define TIMER1_PRESCALE              64
    #define TIMER1_PERIOD                250 // 1ms

    #define TIMER1_TICK_PER_SECOND       (FCY/TIMER1_PRESCALE/TIMER1_PERIOD)
    #define TIMER1_MICROSECONDS_PER_TICK 1000000/TICK_PER_SECOND

    // #define TIMER1_USE_EXTERNAL // uncomment to use external clock
    #define TIMER1_INTERRUPT_MODE // uncomment if you use interrupt mode for timer1

    //* UIP CONFIGURATION ***********************************************
    #define UIP_CONF_MAX_CONNECTIONS 5 // Maximum number of TCP connections.
    #define UIP_CONF_MAX_LISTENPORTS 40 // Maximum number of listening TCP ports.
    #define UIP_CONF_BUFFER_SIZE     400 // uIP buffer size.
    #define UIP_CONF_BYTE_ORDER      LITTLE_ENDIAN // CPU byte order.
    #define UIP_CONF_LOGGING         0 // Logging on or off
    #define UIP_CONF_UDP             0 // UDP support on or off
    #define UIP_CONF_UDP_CHECKSUMS   0 // UDP checksums on or off
    #define UIP_CONF_STATISTICS      0 // uIP statistics on or off
    #define UIP_CONF_BROADCAST	  	 0 // Broadcast support.
    #define UIP_CONF_UDP_CONNS	  	 0 // The maximum amount of concurrent UDP connections.

    #define CLOCK_CONF_SECOND        ((clock_time_t)TIMER1_TICK_PER_SECOND)

    typedef unsigned char u8_t; // the 8-bit type used in uIP
    typedef unsigned char uint8_t; // the 8-bit type used in MQTT
    typedef char int8_t;
    typedef unsigned short u16_t; // the 16-bit type used in uIP
    typedef unsigned short uint16_t; // the 16-bit type used in MQTT
    typedef short int16_t;
    typedef unsigned short uip_stats_t; // dataype used for statistics in uIP
    typedef unsigned int clock_time_t; // clock type in uIP

    //* MQTT CONFIGURATION **********************************************
    #define MQTT_MAX_PACKET_SIZE    128 // Maximum packet size
    #define MQTT_DEFAULT_KEEP_ALIVE  30 // KeepAlive interval in Seconds
    #define MQTTPROTOCOLVERSION       3
    #define MQTT_DEFAULT_PORT      1883 // Default TCP port

    #define MQTT_MAX_CLIENT_ID_LEN   23
    #define MQTT_MAX_PAYLOAD_LEN    127

    //* UART2 CONFIGURATION *********************************************
    #define BAUDRATE2         19200UL
    #define BRG_DIV2          4
    #define BRGH2             1
    #define UART2_FIFO_SIZE   64
    #define UART2_BUFFER_SIZE 64

    #define UART2_PIN_TX 0//7
    #define UART2_PIN_RX 1//8

    #define Init_UART()    UART2Init()
    #define PutChar(c)      UART2PutChar(c)
    #define PutHex(h)       UART2PutHex(h)
    #define PrintString(s)  UART2PrintString(s)

    //* I2C2 CONFIGURATION **********************************************
    #define I2C2_BRG 0x9D
    #define Init_I2C2()     i2c2_init(I2C2_BRG)

    //* SPI2 CONFIGURATION **********************************************
    // spi pin config
    #define SPI2_PIN_SCK 13
    #define SPI2_PIN_SDI 14
    #define SPI2_PIN_SDO 15

    #define Init_SPI()                  spi2_init_debug()
    #define spi_send_byte(byte)         spi2_send_byte(byte)
    #define spi_send_read_byte(byte)	spi2_send_read_byte(byte)

    //* nrf24l01 ********************************************************
    #define NRF24L01_PIN_CE  5
    #define NRF24L01_PIN_CSN 101
    #define NRF24L01_PIN_IRQ 100

    #define NRF24L01_INTERRUPT_MODE
    #define NRF24L01_PLAYLOAD_SIZE  0 // 0 = dynamic

    #define Init_NRF24L01() nrf24l01_init()

    //* nrf24l01 link layer *********************************************
    #define NRF_HEADER_LEN 1
    #define NRF_TX_BUFSIZE (UIP_CONF_BUFFER_SIZE+NRF_HEADER_LEN)

    #define Init_NRF24L01_LINK() nrf_initialize_uip()

    //* heart blink LED *************************************************
    #define BLINK_LED_PIN             11
    #define blink_LED_TOGGLES_PER_SEC 2
    #define Init_BLINK_LED()          blink_LED_init()

    //* IR control ******************************************************
    #define IR_LED_PIN        7
    #define IR_LED_TX_PER_SEC 10
    #define Init_IR()         ir_init()


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */
