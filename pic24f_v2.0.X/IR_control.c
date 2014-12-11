#include "HardwareProfile.h"
#include "IR_control.h"
#include "delay.h"
#include "timer1.h"
#include "uip/uip.h" // For timer API
#include "uart2.h"

#define TOPBIT 0x80000000

typedef void (*ir_handler) (unsigned long data, unsigned int nbits);

struct ir_cmd {
    unsigned long data; // Data to send
    unsigned int nbits; // Number of bytes to send
    ir_handler handler; // Function handler to send data
};

struct ir_cmd ir_cmd_queue[16];
unsigned int ir_head;
unsigned int ir_tail;
struct timer ir_timer;

void ir_handleRC5(unsigned long data, unsigned int nbits);
void ir_handleRC5_ext(unsigned long data, unsigned int nbits);
void ir_handleRC6(unsigned long data, unsigned int nbits);
void ir_handleCanal(unsigned long data, unsigned int nbits);
void ir_handleTest(unsigned long data, unsigned int nbits);


void ir_init()
{
    timer_set(&ir_timer, TIMER1_TICK_PER_SECOND/IR_LED_TX_PER_SEC);
    memset(ir_cmd_queue, 0, 16*sizeof(struct ir_cmd));

    ir_head = 0;
    ir_tail = 0;

// pin config
#ifdef IR_LED_PIN
    #if IR_LED_PIN == 8
        #warning IR_LED_PIN on RB8
        #define IR_LED_CONFREGISTER            TRISB
        #define IR_LED_IOREGISTER              LATB
        #define IR_LED_PINMASK                 0x0100 // RB8
        #define IR_LED_RPORREGISTER            RPOR4bits.RP8R
    #elif IR_LED_PIN == 7
        #warning IR_LED_PIN on RB7
        #define IR_LED_CONFREGISTER            TRISB
        #define IR_LED_IOREGISTER              LATB
        #define IR_LED_PINMASK                 0x0080 // RB7
        #define IR_LED_RPORREGISTER            RPOR3bits.RP7R
    #elif IR_LED_PIN == 10
        #warning IR_LED_PIN on RB10
        #define IR_LED_CONFREGISTER            TRISB
        #define IR_LED_IOREGISTER              LATB
        #define IR_LED_PINMASK                 0x0400 // RB10
        #define IR_LED_RPORREGISTER            RPOR5bits.RP10R
    #else
        #error IR_LED config <IR_LED_PIN> out of range
    #endif
#else
    #error IR_LED config <IR_LED_PIN> not defined
#endif

    set_output(IR_LED_CONFREGISTER, IR_LED_PINMASK);
    IR_LED_RPORREGISTER = 18; // Enable OC1

    // Initialize Output Compare Module
    OC1R = 0;
    OC1RS = 421;
    OC1CON2 = 0x001F;   // Sync with This OC module
    OC1CON1 = 0x1C08;   // Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1)
    //OC1CON1bits.OCTSEL = 7; // Select system clock
    //OC1CON1bits.OCM = 0b110;
    OC1CON1 = OC1CON1 | 0x0006;   // Mode 6, Edge-aligned PWM Mode
}

void ir_push_queue(unsigned long data, unsigned int nbits, ir_handler handler) {
    ir_cmd_queue[ir_head].data = data;
    ir_cmd_queue[ir_head].nbits = nbits;
    ir_cmd_queue[ir_head].handler = handler;

    if(ir_head == ir_tail) {
        // Reset timer if ir module is not used for a long time
        timer_set_ready(&ir_timer, TIMER1_TICK_PER_SECOND/IR_LED_TX_PER_SEC);
    }
    ir_head = (ir_head+1)&0x000f;
}

void ir_sendRC5(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleRC5);
}
void ir_sendRC5_ext(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleRC5_ext);
}
void ir_sendRC6(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleRC6);
}
void ir_sendCanal(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleCanal);
}
void ir_sendTest(unsigned long data, unsigned int nbits) {
    ir_push_queue(data, nbits, ir_handleTest);
}

void ir_periodic()
{
    if(ir_head != ir_tail && timer_expired(&ir_timer)) {
        timer_reset(&ir_timer);
        ir_cmd_queue[ir_tail].handler(ir_cmd_queue[ir_tail].data, ir_cmd_queue[ir_tail].nbits);

        ir_tail = (ir_tail+1)&0x000f;
    }
}

void mark(unsigned int time)
{
    OC1R = 210;
    __delay32(((unsigned long)time)*16UL);
}
void space(unsigned int time)
{
    OC1R = 0;
    __delay32(((unsigned long)time)*16UL);
}

void ir_handleRC5_ext(unsigned long data, unsigned int nbits)
{
  int i;
  // enableIROut(36);
  clock_time_t t1, t2;
  t1 = timer1_get_tick(); 
  data = data << (32 - nbits);
  mark(RC5_T1); // First start bit
  mark(RC5_T1); // Second start bit
  space(RC5_T1); // Second start bit
  for (i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      space(RC5_T1); // 1 is space, then mark
      mark(RC5_T1);
    } 
    else {
      mark(RC5_T1);
      space(RC5_T1);
    }
    data <<= 1;
  }
  space(0); // Turn off at end
  t2 = timer1_get_tick(); 
  sprintf(Buffer, "Done RC5 ext, %d %d -> %d\n", t1, t2, t2-t1);
  PrintString(Buffer);
}
void ir_handleRC5(unsigned long data, unsigned int nbits)
{
  int i;
  // enableIROut(36);
  clock_time_t t1, t2;
  t1 = timer1_get_tick(); 
  data = data << (32 - nbits);
  mark(RC5_T1); // First start bit
  space(RC5_T1); // Second start bit
  mark(RC5_T1); // Second start bit
  for (i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      space(RC5_T1); // 1 is space, then mark
      mark(RC5_T1);
    } 
    else {
      mark(RC5_T1);
      space(RC5_T1);
    }
    data <<= 1;
  }
  space(0); // Turn off at end
  t2 = timer1_get_tick(); 
  sprintf(Buffer, "Done RC5, %d %d -> %d\n", t1, t2, t2-t1);
  PrintString(Buffer);
}

// Caller needs to take care of flipping the toggle bit
void ir_handleRC6(unsigned long data, unsigned int nbits)
{
  int i, t;
  clock_time_t t1, t2;
  t1 = timer1_get_tick(); 
  // enableIROut(36);
  data = data << (32 - nbits);
  mark(RC6_HDR_MARK);
  space(RC6_HDR_SPACE);
  mark(RC6_T1); // start bit
  space(RC6_T1);
  for (i = 0; i < nbits; i++) {
    if (i == 3) {
      // double-wide trailer bit
      t = 2 * RC6_T1;
    } 
    else {
      t = RC6_T1;
    }
    if (data & TOPBIT) {
      mark(t);
      space(t);
    } 
    else {
      space(t);
      mark(t);
    }

    data <<= 1;
  }
  space(0); // Turn off at end
  t2 = timer1_get_tick(); 
  sprintf(Buffer, "Done RC6, %d %d -> %d\n", t1, t2, t2-t1);
  PrintString(Buffer);
}

void ir_handleCanal(unsigned long data, unsigned int nbits)
{
  // 0x458TXX
  // T=0 or 1
  int i;
  // enableIROut(38);
  data = data << (32 - nbits);
  for (i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      mark(CANAL_T1);
      space(CANAL_T1);
    } 
    else {
      space(CANAL_T1);
      mark(CANAL_T1);
    }

    data <<= 1;
  }
  space(0); // Turn off at end
}

void ir_handleTest(unsigned long data, unsigned int nbits)
{
    sprintf(Buffer, "Test %d\n", nbits);
    PrintString(Buffer);
}
