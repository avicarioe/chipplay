#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>

#ifndef LOG_ENABLED
#define LOG_ENABLED 0
#endif

#ifndef LOG_MODULE_NAME
#define LOG_MODULE_NAME "app"
#endif

#define LOG_LEVEL_DEBUG_NAME "debug"
#define LOG_LEVEL_INFO_NAME "info"
#define LOG_LEVEL_WARN_NAME "warn"
#define LOG_LEVEL_ERROR_NAME "error"

#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 1

#ifndef LOG_LEVEL_ENABLED
#define LOG_LEVEL_ENABLED LOG_LEVEL_ERROR
#endif

#if LOG_ENABLED
#define __LOG_INTERNAL(level, module, fmt, ...) \
	printf("<" level ">\t" module ": " fmt "\n\r", ##__VA_ARGS__)
#else
#define __LOG_INTERNAL(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_DEBUG_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_INFO_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_WARN_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_ERROR_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#endif // __LOG_H
