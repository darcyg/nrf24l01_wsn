/*
 * File:   main.c
 * Author: Simon Piriou <spiriou31@gmail.com>
 *
 * Created on 21 octobre 2012, 21:44
 */

#include "main.h"
// timer1 module
#include "timer1.h"
// Link layer module
#include "spi2.h"
#include "nrf24l01/nrf24l01.h"
#include "nrf24l01/nrf24l01_link.h"
// uip stack module
#include "uip/uip.h"
// MQTT client module
#include "mqtt/mqtt.h"
// Simple tcp server (for test)
#include "tcp/tcp_server.h"
// I/O modules
#include "uart2.h"
#include "i2c2.h"
// LED module
#include "blink_LED.h"
// IR LED module
#include "IR_control.h"
// delay module
#include "delay.h"

uint8_t char2hex(char c)
{
    if(c>='0' && c<='9') {
        return (uint8_t)c-'0';
    }
    else if(c>='A' && c<='Z') {
        return (uint8_t)c-'A'+0xa;
    }
    else if(c>='a' && c<='z') {
        return (uint8_t)c-'a'+0xa;
    }
    PrintString("Char Fail\n");
    return (uint8_t)c;
}

// MQTT callback function
void mqtt_message_callback(const char* topic, uint8_t* payload, int payload_length)
{
    static int rc6_toggle=0;
    int i;
    unsigned long data;

    if(!strncmp(topic, "salon/ir", 8)) {
        if(payload_length >= 2) {
            if(payload[0] == 'r') { // RC6 code
                rc6_toggle = rc6_toggle ? 0:1;
                data = 0;
                for(i=1; i<payload_length; i++)
                {
                    if(i>8) {
                        PrintString("i error\n");
                        break;
                    }
                    data |= ((unsigned long)char2hex(payload[i])) << ((payload_length-i-1)*4);
                    // sprintf(Buffer, "data %ld %x %x\n", data, ((uint8_t*)&data)[0], ((uint8_t*)&data)[payload_length-2]);
                    // PrintString(Buffer);
                }
                ir_sendRC6(data, (payload_length-1)*4);
                ir_sendRC6(data, (payload_length-1)*4);
            }
            else if(payload[0] == 's') { // RC5 code
                data = 0;
                for(i=1; i<payload_length; i++)
                {
                    if(i>8) {
                        PrintString("i error\n");
                        break;
                    }
                    data |= ((unsigned long)char2hex(payload[i])) << ((payload_length-i-1)*4);
                    // sprintf(Buffer, "data %ld %x %x\n", data, ((uint8_t*)&data)[0], ((uint8_t*)&data)[payload_length-2]);
                    // PrintString(Buffer);
                }
                ir_sendRC5(data, (payload_length-1)*4);
                ir_sendRC5(data, (payload_length-1)*4);
            }
            else if(payload[0] == 't') { // RC5 7 bits code
                data = 0;
                for(i=1; i<payload_length; i++)
                {
                    if(i>8) {
                        PrintString("i error\n");
                        break;
                    }
                    data |= ((unsigned long)char2hex(payload[i])) << ((payload_length-i-1)*4);
                }
                ir_sendRC5_ext(data, (payload_length-1)*4);
                ir_sendRC5_ext(data, (payload_length-1)*4);
            }
            else if(payload[0] == 'c') { // Canal code
                data = 0;
                for(i=1; i<payload_length; i++)
                {
                    if(i>8) {
                        PrintString("i error\n");
                        break;
                    }
                    data |= ((unsigned long)char2hex(payload[i])) << ((payload_length-i-1)*4);
                }
                ir_sendCanal(data, (payload_length-1)*4-1);
                ir_sendCanal(data, (payload_length-1)*4-1);
            }
        }
    }
    else {
        sprintf(Buffer, "callback %s : %s\n", topic, payload);
        PrintString(Buffer);
    }
}

// MQTT connect and subscribe function
void mqtt_main_connect(struct mqtt_client* mqtt_client)
{
    uip_ipaddr_t ipaddr;
    mqtt_set_blocking_mode(mqtt_client, 1);
    uip_ipaddr(&ipaddr, 192,168,20,1); // MQTT server IP
    mqtt_connect(mqtt_client, ipaddr, HTONS(MQTT_DEFAULT_PORT));
    mqtt_subscribe(mqtt_client, "salon/ir");
    mqtt_subscribe(mqtt_client, "salon/hello"); // Test topic
    mqtt_set_blocking_mode(mqtt_client, 0);
}

double read_temp()
{
    int voltage = 0;
    int i;
    double temp;

    AD1CHS = 4;                    //ADC channel select 4
    AD1CON1bits.ADON =1;           // turn ADC on

    for(i=0; i<8; i++)
    {
        AD1CON1bits.DONE=0;         //resets DONE bit
        AD1CON1bits.SAMP=1;         //start sample
        while(AD1CON1bits.DONE==0); //wait for conversion to finish
        voltage += ADC1BUF0;         //get the voltage measurement
    }
    AD1CON1bits.ADON = 0;           // turn ADC off
    temp = (310.f*(voltage>>3))/1024.f-50.f;

    return temp;
}

int read_luminosity()
{
    int voltage = 0;
    int i;

    AD1CHS = 5;                    // ADC channel select 5
    AD1CON1bits.ADON =1;           // turn ADC on
    for(i=0; i<8; i++)
    {
        AD1CON1bits.DONE=0;         //resets DONE bit
        AD1CON1bits.SAMP=1;         //start sample
        while(AD1CON1bits.DONE==0); //wait for conversion to finish
        voltage += ADC1BUF0;         //get the voltage measurement
    }
    AD1CON1bits.ADON = 0;           // turn ADC off
    return voltage>>3;
}
// Application main function
int main(void)
{
    uip_ipaddr_t ipaddr;
    struct mqtt_client mqtt_client;
    struct timer timer_temp;
    struct timer timer_mqtt_reconnect;

    // Init PLL clock
    InitClock();
    // Configure rx buffer for nrf layer
    nrf_set_rxbuff(uip_buf);
    // Init all i/o
    InitIO();
    // Init timer1
    timer1_init();
    // Start blink led
    blink_restart_irq();
    // Init uip and network stack
    net_init();
    // Init mqtt
    mqtt_init(&mqtt_client, "salut01", mqtt_message_callback);
    
    // Set network parameters
    uip_ipaddr(&ipaddr, 192,168,20,4); // Host IP
    uip_sethostaddr(ipaddr);
    nrf_sethostaddr(ipaddr);
    uip_ipaddr(&ipaddr, 102,168,20,1); // Gateway IP
    uip_setdraddr(ipaddr);
    uip_ipaddr(&ipaddr, 255,255,255,0); // Net mask
    uip_setnetmask(ipaddr);

    PrintString("\r\n*** pic24f tcp/ip node " VERSION " ***\r\n");
    sprintf(Buffer, (const char *)"s=0x%02X, c=0x%02X, t=%02d\r\n",
            nrf24l01_get_status() & 0xFF, nrf24l01_get_config() & 0xFF, CLOCK_SECOND);
    PrintString(Buffer);

    nrf24l01_read_register(nrf24l01_RX_ADDR_P1, (u8_t*)Buffer, 5);
    sprintf(&Buffer[6], (const char *)"addr p1 = %02x %02x %02x %02x %02x\r\n",
                    Buffer[0]&0xff, Buffer[1]&0xff,
                    Buffer[2]&0xff, Buffer[3]&0xff, Buffer[4]&0xff);
    PrintString(&Buffer[6]);

    // Init telnet server
    telnetd_init();
    // Init tcp server
    tcp_hello_init();

    // Connect MQTT client
    mqtt_main_connect(&mqtt_client);

    // Init timer for temperature monitoring
    timer_set(&timer_temp, TIMER1_TICK_PER_SECOND * 10);
    timer_set(&timer_mqtt_reconnect, TIMER1_TICK_PER_SECOND * 20);

    while (1)
    {
        // Handle network communication
        net_poll();

        if(!mqtt_connected(&mqtt_client)) {
            if(timer_expired(&timer_mqtt_reconnect)) {
                blink_set_period(TIMER1_TICK_PER_SECOND/2);
                timer_reset(&timer_mqtt_reconnect);
                // (Re)connect MQTT client
                mqtt_main_connect(&mqtt_client);
            }
            else {
                blink_set_period(TIMER1_TICK_PER_SECOND/10);
            }
        }
        else {
            // Temperature monitoring
            if(timer_expired(&timer_temp)) {
                timer_reset(&timer_temp);
                // PrintString("Publish...\n");
                sprintf(Buffer, "t%02fl%02d", read_temp(), read_luminosity());
                mqtt_publish_string(&mqtt_client, "salon/th", Buffer);
            }
        }

        // Other stuff here
        ir_periodic();
        blink_periodic();
    }
}

void InitIO()
{
    AD1PCFGbits.PCFG = 0xFFFF;   // Set analog pins to digital.
    AD1PCFGbits.PCFG4 = 0; // Clean AN4 for A/D conversion
    AD1PCFGbits.PCFG5 = 0; // Clean AN5 for A/D conversion
    AD1CON1bits.SSRC  = 0b111;     // SSRC<3:0> = 111 implies internal
                                   // counter ends sampling and starts
                                   // converting.
    AD1CON3 = 0x1F02;              // Sample time = 31Tad,
                                   // Tad = 2 Tcy
    // blink led config
    Init_BLINK_LED();
    // UART config
    Init_UART();
    // i2c config
    Init_I2C2();
    // spi config
    Init_SPI();
    // IR config
    Init_IR();
    // nrf24l01 config
    Init_NRF24L01();
    // nrf24l01 config
    Init_NRF24L01_LINK();
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    timer1_irq_handler();
}
/*******************************************************************************
Function:       void __attribute__((__interrupt__, auto_psv)) _DefaultInterrupt(void)

This is just here to catch any interrupts that we see during debugging.

 *******************************************************************************/

void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
    UART2PrintString( "!!! Default interrupt handler !!!\r\n" );
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}
void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
    UART2PrintString( "!!! Oscillator Fail interrupt handler !!!\r\n" );
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
    UART2PrintString( "!!! Address Error interrupt handler !!!\r\n" );
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
    UART2PrintString( "!!! Stack Error interrupt handler !!!\r\n" );
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
    UART2PrintString( "!!! Math Error interrupt handler !!!\r\n" );
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}

void uip_log(char* msg)
{
    sprintf(Buffer, "%s\n\r", msg);
    PrintString(Buffer);
}

