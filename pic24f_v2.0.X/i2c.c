#include "HardwareProfile.h"
#include "i2c2.h"
#include "delay.h"

//function initiates I2C2 module to baud rate BRG
void i2c2_init(int BRG)
{
   // This module uses pins RP8 and RP9
   #warning I2C2_PIN_SCL on RP8
   #warning I2C2_PIN_SDA on RP9
   int temp;

   // I2CBRG = 194 for 10Mhz OSCI with PPL with 100kHz I2C clock
   I2C2BRG = BRG;
   I2C2CONbits.I2CEN = 0;	// Disable I2C Mode
   I2C2CONbits.SCLREL = 1;
   I2C2CONbits.A10M = 0;
   I2C2CONbits.DISSLW = 1;	// Disable slew rate control
   IFS3bits.MI2C2IF = 0;	 // Clear Interrupt
   I2C2CONbits.I2CEN = 1;	// Enable I2C Mode
   temp = I2C2RCV;	 // read buffer to clear buffer full
   i2c2_reset();	 // set bus to idle
}

//function iniates a start condition on bus
void i2c2_start(void)
{
   int x = 0;
   I2C2CONbits.ACKDT = 0;	//Reset any previous Ack
   delay_us(10);
   I2C2CONbits.SEN = 1;	//Initiate Start condition
   Nop();

   //the hardware will automatically clear Start Bit
   //wait for automatic clear before proceding
   while (I2C2CONbits.SEN)
   {
      delay_us(1);
      x++;
      if (x > 20)
      break;
   }
   delay_us(2);
}

void i2c2_restart(void)
{
   int x = 0;

   I2C2CONbits.RSEN = 1;	//Initiate restart condition
   Nop();

   //the hardware will automatically clear restart bit
   //wait for automatic clear before proceding
   while (I2C2CONbits.RSEN) {
      delay_us(1);
      x++;
      if (x > 20)	break;
   }

   delay_us(2);
}

//Resets the I2C bus to Idle
void i2c2_reset(void)
{
   int x = 0;
   //initiate stop bit
   I2C2CONbits.PEN = 1;

   //wait for hardware clear of stop bit
   while (I2C2CONbits.PEN)
   {
      delay_us(1);
      x ++;
      if (x > 20) break;
   }
   I2C2CONbits.RCEN = 0;
   IFS3bits.MI2C2IF = 0; // Clear Interrupt
   I2C2STATbits.IWCOL = 0;
   I2C2STATbits.BCL = 0;
   delay_us(10);
}

//basic I2C byte send
char i2c2_write_byte(int data)
{
   int i;

   while (I2C2STATbits.TBF) { }
   IFS3bits.MI2C2IF = 0; // Clear Interrupt
   I2C2TRN = data; // load the outgoing data byte

   // wait for transmission
   for (i=0; i<500; i++)
   {
      if (!I2C2STATbits.TRSTAT) break;
      delay_us(1);

      }
      if (i == 500) {
      return(1);
   }

   // Check for NO_ACK from slave, abort if not found
   if (I2C2STATbits.ACKSTAT == 1)
   {
      i2c2_reset();
      return(1);
   }

   delay_us(2);
   return(0);
}

unsigned int i2c2_receive_data(unsigned int length, char * rdptr, unsigned int i2c2_data_wait)
{
    int wait = 0;
    while(length)                    /* Receive length bytes */
    {
        I2C2CONbits.RCEN = 1;
        while(!I2C2STATbits.RBF)
        {
            if(wait < i2c2_data_wait)
                wait++;
            else
            return(length);          /* Time out,
                                        return number of byte/word to be read */
        }
        wait = 0;
        *rdptr = I2C2RCV;            /* save byte received */
        rdptr++;
        length--;
        if(length) // If not last char, generate ACK sequence
        {
            I2C2CONbits.ACKEN = 1;
            while(I2C2CONbits.ACKEN == 1);    // Wait till ACK sequence
        }
    }
    return 0;
}


//function reads data, returns the read data, no ack
char i2c2_read_byte(void)
{
   int i = 0;
   char data = 0;
   //set I2C module to receive
   I2C2CONbits.RCEN = 1;
   //if no response, break
   while (!I2C2STATbits.RBF)
   {
      i ++;
      if (i > 2000) break;
   }
   //get data from I2CRCV register
   data = I2C2RCV;
   return data;
}

//function reads data, returns the read data, with ack
char i2c2_read_ack_byte(void)	//does not reset bus!!!
{
   int i = 0;
   char data = 0;

   //set I2C module to receive
   I2C2CONbits.RCEN = 1;

   //if no response, break
   while (!I2C2STATbits.RBF)
   {
      i++;
      if (i > 2000) break;
   }

   //get data from I2CRCV register
   data = I2C2RCV;

   //set ACK to high
   I2C2CONbits.ACKEN = 1;
   // Wait till ACK/NACK sequence
   while(I2C2CONbits.ACKEN == 1);

   return data;
}

int i2c2_write(char addr, char subaddr, char value)
{
   i2c2_start();
   // Test if the device exists
   if(i2c2_write_byte(addr))
   {
       i2c2_reset();
       return 0;
   }
   i2c2_write_byte(0);
   i2c2_write_byte(subaddr);
   i2c2_write_byte(value);
   i2c2_reset();
   return 1;
}

int i2c2_write_array(char addr, char subaddr, int length, char* values)
{
   i2c2_start();
   // Test if the device exists
   if(i2c2_write_byte(addr))
   {
       i2c2_reset();
       return 0;
   }
   i2c2_write_byte(0);
   i2c2_write_byte(subaddr);

   while(length > 0)
   {
       i2c2_write_byte(*values);
       values++;
       length--;
   }
   i2c2_reset();
   return 1;
}

char i2c2_read(char addr, char subaddr)
{
   char temp;

   i2c2_start();
   i2c2_write_byte(addr);
   i2c2_write_byte(0);
   i2c2_write_byte(subaddr);
   delay_us(10);

   i2c2_restart();
   i2c2_write_byte(addr | 0x01);
   temp = i2c2_read_byte();

   i2c2_reset();
   return temp;
}

int i2c2_receive(char addr, char subaddr, unsigned int length, char * rdptr)
{
   char temp;

   i2c2_start();
   if(i2c2_write_byte(addr))
   {
       i2c2_reset();
       return 1;
   }
   i2c2_write_byte(0);
   i2c2_write_byte(subaddr);
   delay_us(10);

   i2c2_restart();
   i2c2_write_byte(addr | 0x01);
   temp = i2c2_receive_data(length, rdptr, 2000);
   i2c2_reset();
   return temp;
}

unsigned char i2c2_poll(char addr)
{
   unsigned char temp = 0;

   i2c2_start();
   temp = i2c2_write_byte(addr);
   i2c2_reset();

   return temp;
}
