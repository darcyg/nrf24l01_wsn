#ifndef _UART_H
#define _UART_H

#include "FreeRTOS.h"
#include <stdbool.h>

void uart2_init();
void uart2_print(char *str);
void uart2_putc(char ch);
void uart2_puts(char* buf, unsigned int len);
bool uart2_getc(char *pchar, TickType_t timeout);

#define uart2_printf(format, ...) \
    { char printk_buf[64]; \
      sprintf(printk_buf, format, ## __VA_ARGS__); \
      uart2_print(printk_buf); }

void UART2ClrError(void);

#endif
