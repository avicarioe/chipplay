#ifndef TIMEOUT_H__
#define TIMEOUT_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct timeout_t {
	uint32_t start;
	uint32_t ms;
} timeout_t;

void timeout_init();
void timeout_start(timeout_t* timeout, uint32_t ms);
bool timeout_check(timeout_t* timeout);
void timeout_delay(uint32_t ms);
uint32_t timeout_count();

#endif // TIMEOUT_H__
