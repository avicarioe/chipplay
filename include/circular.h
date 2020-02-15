#ifndef CIRCULAR_H__
#define CIRCULAR_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"

typedef struct circular_t {
	uint8_t* data;
	uint32_t len;
	volatile uint32_t read;
	volatile uint32_t write;
} circular_t;

void circular_init(circular_t* self, uint8_t* data, uint32_t len);
uint32_t circular_used(circular_t* self);
uint8_t circular_get(circular_t* self);
uint32_t circular_free(circular_t* self);
err_t circular_write(circular_t* self, const uint8_t* data, uint32_t len);
void circular_clear(circular_t* self);
err_t circular_read(circular_t* self, uint8_t* data, uint32_t len);
err_t circular_peek(circular_t* self, uint8_t* data, uint32_t len);
void circular_skip(circular_t* self, uint32_t len);
void circular_skip_one(circular_t* self);
void circular_add(circular_t* self, uint8_t data);

#endif // CIRCULAR_H__
