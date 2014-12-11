/*
 * File:   spi2.h
 * Author: Simon
 *
 * Created on 2 décembre 2012, 20:27
 */

#ifndef spi2_H
#define spi2_H

#define SPI_SPRE_1 0x001C
#define SPI_SPRE_2 0x0018
#define SPI_SPRE_3 0x0014
#define SPI_SPRE_4 0x0010
#define SPI_SPRE_5 0x000C
#define SPI_SPRE_6 0x0008
#define SPI_SPRE_7 0x0004
#define SPI_SPRE_8 0x0000

#define SPI_PPRE_1  0x0003
#define SPI_PPRE_4  0x0002
#define SPI_PPRE_16 0x0001
#define SPI_PPRE_64 0x0000

#ifdef	__cplusplus
extern "C" {
#endif

    // init pin
void spi2_pin_init();

// init with prescale
void spi2_init(unsigned int prescale);

// default init
void spi2_init_debug();

void spi2_send_byte(char c);

char spi2_send_read_byte(char c);

void reset_ticket(unsigned int ticketNumber);

#ifdef	__cplusplus
}
#endif


#endif
