#include "HardwareProfile.h"
#include "pic24f_uart2.h"

#include "queue.h"
#include "task.h"

//******************************************************************************
// Constants
//******************************************************************************

//U2BRG register value and baudrate mistake calculation

#if defined (__C30__)
    #if defined (__dsPIC33E__) || defined (__PIC24E__)
    #define BAUDRATEREG2        (((GetSystemClock()) / (BRG_DIV2 * BAUDRATE2)) - 1)
    #else
    #define BAUDRATEREG2        (((GetSystemClock() / 2) + (BRG_DIV2 / 2 * BAUDRATE2)) / BRG_DIV2 / BAUDRATE2 - 1)
    #endif
#elif defined (__PIC32MX__)
    #define BAUDRATEREG2        ((GetPeripheralClock() + (BRG_DIV2 / 2 * BAUDRATE2)) / BRG_DIV2 / BAUDRATE2 - 1)
#else
    #error Cannot calculate BRG value
#endif

#if defined (__C30__)
    #if defined (__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
    #define BAUD_ACTUAL         ((GetSystemClock()) / (BRG_DIV2 * (BAUDRATEREG2 + 1)))
    #else
    #define BAUD_ACTUAL         ((GetSystemClock() / 2) / BRG_DIV2 / (BAUDRATEREG2 + 1))
    #endif
#elif defined (__PIC32MX__)
    #define BAUD_ACTUAL         (GetPeripheralClock() / BRG_DIV2 / (BAUDRATEREG2 + 1))
#else
    #error Cannot calculate actual baud rate
#endif


    #define BAUD_ERROR              ((BAUD_ACTUAL > BAUDRATE2) ? BAUD_ACTUAL - BAUDRATE2 : BAUDRATE2 - BAUD_ACTUAL)
    #define BAUD_ERROR_PERCENT      ((BAUD_ERROR * 100 + BAUDRATE2 / 2) / BAUDRATE2)

    #if (BAUD_ERROR_PERCENT > 3)
        #error UART frequency error is worse than 3%
    #elif (BAUD_ERROR_PERCENT > 2)
        #warning UART frequency error is worse than 2%
    #endif

static QueueHandle_t xRxedChars;

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
    char cChar;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Get the character and post it on the queue of Rxed characters.
       If the post causes a task to wake force a context switch as the woken task
       may have a higher priority than the task we have interrupted. */
    IFS1bits.U2RXIF = 0;
    while ( U2STAbits.URXDA ) {
        cChar = U2RXREG;
        xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
    }

    if ( xHigherPriorityTaskWoken != pdFALSE ) {
        taskYIELD();
    }
}

void uart2_init()
{
    U2BRG = BAUDRATEREG2;
    U2MODE = 0;
    U2MODEbits.BRGH = BRGH2;
    U2STA = 0;
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;

    IPC7bits.U2RXIP = configKERNEL_INTERRUPT_PRIORITY;
    IFS1bits.U2RXIF = 0;    // Clear the Recieve Interrupt Flag
    IEC1bits.U2RXIE = 1;    // Enable Receive Interrupts

    xRxedChars = xQueueCreate(UART2_FIFO_SIZE, (unsigned portBASE_TYPE)sizeof(signed char));
}

void uart2_print( char *str )
{
    unsigned char c;

    while ( (c = *str++) )
        uart2_putc(c);
}

void uart2_putc(char ch)
{
    U2TXREG = ch;
    Nop();
    while (U2STAbits.TRMT == 0);
}

void uart2_puts(char* buf, unsigned int len)
{
    for (len += (unsigned int)buf; len != (unsigned int)buf; buf++) {
        uart2_putc(*buf);
    }
}

bool uart2_getc(char *pchar, TickType_t timeout)
{
    if (xQueueReceive(xRxedChars, pchar, timeout)) {
        return true;
    }
    return false;
}

void UART2ClrError(void)
{
    // Clear error flag
    if (U2STAbits.OERR)
        U2STAbits.OERR = 0;
}
