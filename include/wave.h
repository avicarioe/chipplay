#ifndef WAVE_H__
#define WAVE_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"

typedef struct wave_t {
	uint32_t length;
	uint32_t position;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t align;
	uint16_t bpsample;
} wave_t;

typedef struct wave_io_t {
	uint8_t* in;
	uint32_t in_len;
	uint8_t* out;
	uint32_t out_len;
} wave_io_t;

 err_t wave_init(wave_t* wave, uint8_t* buff, uint32_t len, uint16_t* next);
 uint16_t wave_dec(wave_t* wave, wave_io_t* io, uint16_t* next);

#endif // WAVE_H__
