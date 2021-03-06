/*
@file: wave.c
Copyright (C) 2020 by Alejandro Vicario, Xiaoyu Wang and chipPLAY contributors.
This file is part of the chipPLAY project.
ChipPLAY is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ChipPLAY is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with ChipPlay.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wave.h"
#include "util.h"

#define FORMAT_PCM 1

#define STR_RIFF   0x46464952
#define STR_WAVE   0x45564157
#define STR_FMT    0x20746D66
#define STR_DATA   0x61746164

typedef union {
	char str[4];
	uint32_t value;
} wave_str_t;

typedef struct wave_chunk_t wave_chunk_t;
typedef struct wave_sub_t wave_sub_t;
typedef struct wave_fmt_t wave_fmt_t;


struct wave_chunk_t {
	wave_str_t riff;
	uint32_t chunk_size;
	wave_str_t wave;
} __attribute__((packed));

struct wave_sub_t {
	wave_str_t type;
	uint32_t size;
} __attribute__((packed));

struct wave_fmt_t {
	wave_sub_t sub;
	uint16_t format;
	uint16_t nch;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t align;
	uint16_t bpsample;
} __attribute__((packed));


/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static bool check_chunk(circular_t* in);
static bool process_fmt(wave_t* wave, wave_fmt_t* fmt);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static bool check_chunk(circular_t* in)
{
	wave_chunk_t chunk;

	if(circular_read(in, (uint8_t*)&chunk, sizeof(chunk)) != SUCCESS) {
		return false;
	}

	if(chunk.riff.value != STR_RIFF) {
		return false;
	}

	if(chunk.wave.value != STR_WAVE) {
		return false;
	}

	return true;
}

static bool process_fmt(wave_t* wave, wave_fmt_t* fmt)
{

	if(fmt->format != FORMAT_PCM) {
		return false;
	}

	wave->channels = fmt->nch;
	wave->sample_rate = fmt->sample_rate;
	wave->byte_rate = fmt->byte_rate;
	wave->align = fmt->align;
	wave->bpsample = fmt->bpsample / 8;

	return true;
}

/** Public functions **********************************************************/
err_t wave_init(wave_t* wave, circular_t* in, circular_t* pcm)
{
	wave->in = in;
	wave->pcm = pcm;

	if(!check_chunk(in)) {
		return ERR_INVALD_DATA;
	}

	wave_sub_t sub;
	RETURN_ON_ERROR(circular_read(in, (uint8_t*)&sub, sizeof(sub)));


	while(sub.type.value != STR_DATA) {

		if(sub.type.value == STR_FMT) {
			wave_fmt_t fmt;
			fmt.sub = sub;

			RETURN_ON_ERROR(circular_read(in, (uint8_t*) & (fmt.format),
					fmt.sub.size));

			if(!process_fmt(wave, &fmt)) {
				return ERR_INVALD_DATA;
			}
		} else {
			circular_skip(in, sub.size);
		}

		RETURN_ON_ERROR(circular_read(in, (uint8_t*)&sub, sizeof(sub)));
	}

	if(wave->channels > 2) {
		return ERR_NOT_SUPPORTED;
	}

	wave->length = sub.size;
	wave->position = 0;

	return SUCCESS;
}

uint16_t wave_dec(wave_t* wave)
{
	if(wave_end(wave)) {
		return 0;
	}

	uint16_t samples = circular_used(wave->in) / wave->align;

	uint32_t out_free = circular_free(wave->pcm) / 2;
	if(out_free < samples) {
		samples = out_free;
	}

	uint32_t left = (wave->length - wave->position) / wave->align;
	if(left < samples) {
		samples = left;
	}

	for(int i = 0; i < samples; i++) {
		int8_t left = 0;
		int8_t right = 0;


		circular_skip(wave->in, wave->bpsample - 1);
		left = circular_get(wave->in);

		if(wave->channels >= 2) {
			circular_skip(wave->in, wave->bpsample - 1);
			right = circular_get(wave->in);
		} else {
			right = left;
		}


		if(wave->bpsample > 1) {
			left += 128;
			right += 128;
		}

		circular_add(wave->pcm, (uint8_t)left);
		circular_add(wave->pcm, (uint8_t)right);

	}

	wave->position += samples * wave->align;
	return samples;
}

bool wave_end(wave_t* wave)
{
	return wave->position >= wave->length;
}

uint8_t wave_progress(wave_t* wave)
{
	uint32_t pos = wave->position;
	pos *= 0xFF;
	return pos / wave->length;
}
