#include "logger.h"
#include "HardwareProfile.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

// Flush on uart2
#include "pic24f_uart2.h"

#define MAGIC_KEY 'K'

#define LOGGER_DEF_LEVEL(lvl, msg) [lvl] = msg,
static char* log_levels[] = {
#include "logger_levels.h"
};
#undef LOGGER_DEF_LEVEL

struct cbuffer {
    char *w;
    char *r;
    char buffer[LOG_BUFFER_SIZE];
};

// No need for magic key as the buffer is always in a determinitic state (PANIC if saturation)
struct __attribute__((__packed__)) msg_header {
    uint16_t timestamp;
    uint8_t level;
    uint8_t size;
};

static struct cbuffer log_buf;

static struct cbuffer log_buf = {
    .w = log_buf.buffer,
    .r = log_buf.buffer
};

void log_printk(uint8_t level, const char* format, ...)
{
    int written;
    char* write_ptr;
    va_list args;

    char tmp_buf[LOG_MAX_MSG_SIZE + sizeof(struct msg_header)];

    va_start(args, format);
    written = vsprintf(tmp_buf + sizeof(struct msg_header), format, args);
    if (written <= 0) {
        // ERROR
        return;
    }
    // ((struct msg_header*)tmp_buf)->magic_key = MAGIC_KEY;
    ((struct msg_header*)tmp_buf)->size = written; // (--written); // Remove trailing zero
    ((struct msg_header*)tmp_buf)->level = level;
    ((struct msg_header*)tmp_buf)->timestamp = xTaskGetTickCount();

    written += sizeof(struct msg_header); // Add msg header size

    portENTER_CRITICAL();
    // Update write ptr
    write_ptr = log_buf.w;
    log_buf.w = log_buf.buffer + (unsigned int)(((log_buf.w - log_buf.buffer) + written) & (LOG_BUFFER_SIZE - 1));
    portEXIT_CRITICAL();

    // Copy tmp_buf into circular buffer
    if (log_buf.w < write_ptr) {
        // Buffer overflow detected
        unsigned int remaining_bytes = (LOG_BUFFER_SIZE)-(write_ptr - log_buf.buffer);
        memcpy(write_ptr, tmp_buf, remaining_bytes);
        memcpy(log_buf.buffer, tmp_buf + remaining_bytes, written - remaining_bytes);
    } else {
        memcpy(write_ptr, tmp_buf, written);
    }
}

void log_flush_msg()
{
    char buffer[8];

    if (log_buf.r != log_buf.w) {
        uint16_t timestamp = 0;
        // printk("flush %x %x %x\n\r", (unsigned int)log_buf.buffer, (unsigned int)log_buf.w, (unsigned int)log_buf.r);
        // Message in buffer
        timestamp = log_buf.r[0] & 0x00FF;
        log_buf.r = log_buf.buffer + (unsigned int)(((log_buf.r - log_buf.buffer) + 1) & (LOG_BUFFER_SIZE - 1));
        timestamp = timestamp + (log_buf.r[0] << 8);
        log_buf.r = log_buf.buffer + (unsigned int)(((log_buf.r - log_buf.buffer) + 1) & (LOG_BUFFER_SIZE - 1));

//         if (timestamp < 10) goto ts_10;
//         if (timestamp < 100) goto ts_100;
//         if (timestamp < 1000) goto ts_1000;
//         if (timestamp < 10000) goto ts_10000;
//
//         goto ts_done;
// ts_10:
//         uart2_putc(' ');
// ts_100:
//         uart2_putc(' ');
// ts_1000:
//         uart2_putc(' ');
// ts_10000:
//         uart2_putc(' ');
// ts_done:

        sprintf(buffer, "%u ", timestamp);
        uart2_print(buffer);
        uart2_putc('[');
        uart2_print(log_levels[(uint8_t)(log_buf.r[0])]);
        uart2_print("] ");
        log_buf.r = log_buf.buffer + (unsigned int)(((log_buf.r - log_buf.buffer) + 1) & (LOG_BUFFER_SIZE - 1));
        uint8_t size = log_buf.r[0];
        log_buf.r = log_buf.buffer + (unsigned int)(((log_buf.r - log_buf.buffer) + 1) & (LOG_BUFFER_SIZE - 1));

        if (log_buf.r + size > log_buf.buffer + LOG_BUFFER_SIZE) {
            unsigned int remaining_bytes = (LOG_BUFFER_SIZE)-(log_buf.r - log_buf.buffer);
            uart2_puts(log_buf.r, remaining_bytes);
            uart2_puts(log_buf.buffer, size - remaining_bytes);
        } else {
            uart2_puts(log_buf.r, size);
        }
        uart2_putc('\n');
        uart2_putc('\r');

        log_buf.r = log_buf.buffer + (unsigned int)(((log_buf.r - log_buf.buffer) + size) & (LOG_BUFFER_SIZE - 1));
    }
}

int log_is_empty()
{
    return !!(log_buf.r == log_buf.w);
}
