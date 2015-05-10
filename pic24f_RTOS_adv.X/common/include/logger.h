#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdint.h>
#include <stdbool.h>

#define LOGGER_DEF_LEVEL(lvl, msg) lvl,
typedef enum {
#include "logger_levels.h"
} LOGGER_LEVEL;
#undef LOGGER_DEF_LEVEL

#define log_info(msg, ...)  log_printk(LVL_INFO,  msg, ## __VA_ARGS__)
#ifdef DEBUG
#define log_debug(msg, ...) log_printk(LVL_DEBUG, msg, ## __VA_ARGS__)
#else
#define log_debug(msg, ...)
#endif
#define log_error(msg, ...) log_printk(LVL_ERROR, msg, ## __VA_ARGS__)

void log_printk(uint8_t level, const char* format, ...);

void log_flush_msg();

bool log_is_empty();

#endif
