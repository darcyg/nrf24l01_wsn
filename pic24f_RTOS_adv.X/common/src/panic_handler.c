#include "board_config.h"
#include "pic24f_uart2.h"
#include "FreeRTOS.h"
#include "task.h"

#define SPDUMP_MAX  64   // Maximum stack to dump

extern int wRegSave[16];
extern int TrapAddr;
extern int ErrorAddrHigh;
extern int ErrorAddrLow;
extern int _SP_init;
extern int _SPLIM_init;

void vApplicationIdleHook(void);

void panic_handler(void)
{
    unsigned int i;
    int *ptr;

    uart2_print("\r\n!!! PANIC ");
    uart2_printf("(%d - 0x%x) at 0x%x", TrapAddr, INTCON1, ErrorAddrLow);
    uart2_print(" !!! task ");
    uart2_print(pcTaskGetTaskName(xTaskGetCurrentTaskHandle()));
    uart2_print("\n\r");

    for (i = 0; i < 16; i++) {
        if ((i & 7) == 0) {
            uart2_print("\n\rw");
            uart2_putc('0' + i);
            uart2_putc(':');
        }
        uart2_printf(" 0x%04x", (uint16_t)wRegSave[i]);
    }
    uart2_print("\r\n");

    ptr = (int*)(&i) - (SPDUMP_MAX / 2);
    for (i = 0; i < SPDUMP_MAX; i++) {
        if ((i & 7) == 0) {
            uart2_print("\n\r0x");
            uart2_printf("%x", (int)ptr);
            uart2_putc(':');
        }
        uart2_print(" 0x");
        uart2_printf("%04x", (*ptr++));
        // if (ptr > (int *)wRegSave[15]) break;
    }

    uart2_print("\n\n\rlogger:\r\n");
    // Flush logger
    vApplicationIdleHook();
    while (1) {
        Nop();
        Nop();
        Nop();
    }
}
