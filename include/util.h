#ifndef __UTIL_H
#define __UTIL_H

#include "log.h"
#include <stdint.h>

#define UNUSED_VARIABLE(X)  ((void)(X))

typedef enum {
	SUCCESS = 0,
	ERR_NULL,
	ERR_INVALID_PARAM,
	ERR_NO_MEM,
	ERR_INVALID_STATE,
	ERR_ASSERT_FAIL,
	ERR_INVALD_DATA,
	ERR_NOT_IMPLEMENTED,
	ERR_NOT_SUPPORTED,
	ERR_INTERNAL,
	ERR_TIMEOUT,
} err_t;

extern void error_fault_handler(err_t err, uint32_t line, const char* file);

#define ERROR_CHECK(__error_code)                                   \
	do                                                              \
	{                                                               \
		const uint32_t __local = (__error_code);                    \
		if (__local != SUCCESS)                                     \
		{                                                           \
			error_fault_handler(                                    \
				__local, __LINE__, (const char *) __FILE__); \
		}                                                           \
	} while (0)

#define RETURN_ON_ERROR(status)     \
	do {                            \
		uint32_t __local = status;  \
		if (__local != SUCCESS)     \
		{                           \
			return __local;         \
		}                           \
	} while (0);


#ifdef DEBUG
#define ASSERT(cond)                                                  \
	do {                                                              \
		uint32_t __local = (cond);                                    \
		if (!__local) {                                               \
			error_fault_handler(ERR_ASSERT_FAIL, __LINE__, __FILE__); \
		}                                                             \
	} while (0)
#else
#define ASSERT(cond)
#endif


#endif // __UTIL_H
