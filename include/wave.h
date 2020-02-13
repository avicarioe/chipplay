#ifndef WAVE_H__
#define WAVE_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "circular.h"

typedef struct wave_t {
	uint32_t length;
	uint32_t position;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t align;
	uint16_t bpsample;
	circular_t* pcm;
	circular_t* in;
} wave_t;

err_t wave_init(wave_t* wave, circular_t* in, circular_t* pcm);
uint16_t wave_dec(wave_t* wave);
bool wave_end(wave_t* wave);
uint8_t wave_progress(wave_t* wave);

#endif // WAVE_H__
