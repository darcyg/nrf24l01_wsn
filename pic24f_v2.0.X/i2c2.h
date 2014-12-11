/* 
 * File:   i2c.h
 * Author: Simon
 *
 * Created on 22 octobre 2012, 19:53
 */

#ifndef I2C2_H
#define	I2C2_H

#ifdef	__cplusplus
extern "C" {
#endif

void i2c2_init(int BRG); //function initiates I2C1 module to baud rate BRG
void i2c2_start(void); //function iniates a start condition on bus
void i2c2_restart(void);
void i2c2_reset(void); //Resets the I2C bus to Idle

char i2c2_write_byte(int data); // basic I2C byte send
char i2c2_read_byte(void); //function reads data, returns the read data, no ack
char i2c2_read_ack_byte(void); // function reads data, returns the read data, with ack
unsigned int i2c2_receive_data(unsigned int length, char * rdptr, unsigned int i2c2_data_wait);

int i2c2_write(char addr, char subaddr, char value);
int i2c2_write_array(char addr, char subaddr, int length, char* values);
char i2c2_read(char addr, char subaddr);
int i2c2_receive(char addr, char subaddr, unsigned int length, char * rdptr);
unsigned char i2c2_poll(char addr);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

