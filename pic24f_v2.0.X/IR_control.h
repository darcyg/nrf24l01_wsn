/* 
 * File:   IR_control.h
 * Author: Simon
 *
 * Created on 24 mars 2013, 20:15
 */

#ifndef IR_CONTROL_H
#define	IR_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

// extern unsigned char rc6_address;
// extern unsigned char rc6_fields;
// extern unsigned char canal_address;

//function initiates IR module
void ir_init();

inline void ir_perodic();

// void ir_sendRaw(unsigned int buf[], int len, int hz);

void ir_sendRC5(unsigned long data, unsigned int nbits);
void ir_sendRC6(unsigned long data, unsigned int nbits);
void ir_sendCanal(unsigned long data, unsigned int nbits);
void ir_sendTest(unsigned long data, unsigned int nbits);

#define RC5_T1		889
#define RC5_RPT_LENGTH	46000

#define RC6_HDR_MARK	2666
#define RC6_HDR_SPACE	889
#define RC6_T1		444
#define RC6_RPT_LENGTH	46000

#define CANAL_T1		260

#ifdef	__cplusplus
}
#endif

#endif	/* IR_CONTROL_H */

