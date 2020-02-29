/*
@file: wave.h
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
