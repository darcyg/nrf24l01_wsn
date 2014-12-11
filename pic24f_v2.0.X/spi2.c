
#include "Compiler.h"
#include "HardwareProfile.h"
#include "spi2.h"

void spi2_pin_init()
{
#ifdef SPI2_PIN_SCK
    #if SPI2_PIN_SCK == 13
        #warning SPI2_PIN_CLK on RP13
        #ifdef PIN_IO_13
            #error PIN_IO_13 already used
        #else
            #define PIN_IO_13 1
            RPOR6bits.RP13R = 11;
        #endif
    #elif SPI2_PIN_SCK == 14
        #warning SPI2_PIN_CLK on RP14
        #ifdef PIN_IO_14
            #error PIN_IO_14 already used
        #else
            #define PIN_IO_14 1
            RPOR7bits.RP14R = 11;
        #endif
    #elif SPI2_PIN_SCK == 15
        #warning SPI2_PIN_CLK on RP15
        #ifdef PIN_IO_15
            #error PIN_IO_15 already used
        #else
            #define PIN_IO_15 1
            RPOR7bits.RP15R = 11;
        #endif
    #else
        #error SPI config <SPI2_PIN_SCK> out of range
    #endif
#else
    #error SPI config <SPI2_PIN_SCK> not defined
#endif

#ifdef SPI2_PIN_SDO
    #if SPI2_PIN_SDO == 13
        #warning SPI2_PIN_SDO on RP13
        #ifdef PIN_IO_13
            #error PIN_IO_13 already used
        #else
            #define PIN_IO_13 1
            RPOR6bits.RP13R = 10;
        #endif
    #elif SPI2_PIN_SDO == 14
        #warning SPI2_PIN_SDO on RP14
        #ifdef PIN_IO_14
            #error PIN_IO_14 already used
        #else
            #define PIN_IO_14 1
            RPOR7bits.RP14R = 10;
        #endif
    #elif SPI2_PIN_SDO == 15
        #warning SPI2_PIN_SDO on RP15
        #ifdef PIN_IO_15
            #error PIN_IO_15 already used
        #else
            #define PIN_IO_15 1
            RPOR7bits.RP15R = 10;
        #endif
    #else
        #error SPI config <SPI2_PIN_SDO> out of range
    #endif
#else
    #error SPI config <SPI2_PIN_SDO> not defined
#endif

#ifdef SPI2_PIN_SDI
    #if SPI2_PIN_SDI == 13
        #warning SPI2_PIN_SDI on RP13
        #ifdef PIN_IO_13
            #error PIN_IO_13 already used
        #else
            #define PIN_IO_13 1
            RPINR22bits.SDI2R = 13;
        #endif
    #elif SPI2_PIN_SDI == 14
        #warning SPI2_PIN_SDI on RP14
        #ifdef PIN_IO_14
            #error PIN_IO_14 already used
        #else
            #define PIN_IO_14 1
            RPINR22bits.SDI2R = 14;
        #endif
    #elif SPI2_PIN_SDI == 15
        #warning SPI2_PIN_SDI on RP15
        #ifdef PIN_IO_15
            #error PIN_IO_15 already used
        #else
            #define PIN_IO_15 1
            RPINR22bits.SDI2R = 15;
        #endif
    #else
        #error SPI config <SPI2_PIN_SDI> out of range
    #endif
#else
    #error SPI config <SPI2_PIN_SDI> not defined
#endif
}

void spi2_init(unsigned int prescale)
{
    // init SPI pin
    spi2_pin_init();

    // init SPI module
    IFS2bits.SPI2IF = 0;
    IEC2bits.SPI2IE = 0; // disable interrupt 1;
    IPC8bits.SPI2IP = 1; // lowest priority

    // CKE = 1: Change SDO on CLK transition from low to high
    // CKP = 0: CLK active on high level
    SPI2CON1 = 0x0120 | (prescale & 0x001F);

    SPI2STATbits.SPIROV = 0;
    SPI2STATbits.SPIEN = 1;
    // End init SPI
}

void spi2_init_debug()
{
    // init SPI debug : Fsck=16M/16/1 = 1M
    spi2_init(SPI_SPRE_1 | SPI_PPRE_16);
}

void spi2_send_byte(char c)
{
    while(SPI2STATbits.SPITBF);
    SPI2BUF = c;
}

char spi2_send_read_byte(char c)
{
    while(SPI2STATbits.SPITBF);
    SPI2BUF = c;
    while(!SPI2STATbits.SPIRBF);
    return SPI2BUF;
}
