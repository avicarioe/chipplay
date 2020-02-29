/*
@file: fft8.c
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

#include "fft8.h"

/** Global variables **********************************************************/
static const int8_t k_lutr[] = {
	127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,
	127,  117,   90,   49,    0,  -49,  -90, -117, -127, -117,  -90,  -49,   -0,   49,   90,  117,
	127,   90,    0,  -90, -127,  -90,   -0,   90,  127,   90,    0,  -90, -127,  -90,   -0,   90,
	127,   49,  -90, -117,   -0,  117,   90,  -49, -127,  -49,   90,  117,    0, -117,  -90,   49,
	127,    0, -127,   -0,  127,    0, -127,   -0,  127,    0, -127,   -0,  127,   -0, -127,   -0,
	127,  -49,  -90,  117,    0, -117,   90,   49, -127,   49,   90, -117,   -0,  117,  -90,  -49,
	127,  -90,   -0,   90, -127,   90,    0,  -90,  127,  -90,   -0,   90, -127,   90,   -0,  -90,
	127, -117,   90,  -49,   -0,   49,  -90,  117, -127,  117,  -90,   49,   -0,  -49,   90, -117,
};

static const int8_t k_luti[] = {
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,  -49,  -90, -117, -127, -117,  -90,  -49,    0,   49,   90,  117,  127,  117,   90,   49,
	0,  -90, -127,  -90,    0,   90,  127,   90,    0,  -90, -127,  -90,    0,   90,  127,   90,
	0, -117,  -90,   49,  127,   49,  -90, -117,    0,  117,   90,  -49, -127,  -49,   90,  117,
	0, -127,    0,  127,    0, -127,    0,  127,    0, -127,    0,  127,    0, -127,    0,  127,
	0, -117,   90,   49, -127,   49,   90, -117,    0,  117,  -90,  -49,  127,  -49,  -90,  117,
	0,  -90,  127,  -90,    0,   90, -127,   90,    0,  -90,  127,  -90,    0,   90, -127,   90,
	0,  -49,   90, -117,  127, -117,   90,  -49,    0,   49,  -90,  117, -127,  117,  -90,   49,
};

static const uint8_t log_lut[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8
};

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions **********************************************************/
void ftt8_calc(const uint8_t* in, uint8_t* out)
{

	for(int i = 0; i < 8; i++) {
		int16_t sum_r = 0;
		int16_t sum_i = 0;
		uint8_t row = i * 16;

		for(int j = 0; j < 16; j++) {
			int16_t ins = in[j] - 128;
			int16_t kr = k_lutr[row + j];
			int16_t ki = k_luti[row + j];
			sum_r += (ins * kr) / 128 * 5;
			sum_i += (ins * ki) / 128 * 5;
		}

		uint32_t sum_buff = (sum_r * sum_r + sum_i * sum_i);
		sum_buff = sum_buff >> 12;
		out[i] = log_lut[(sum_buff > 255) ? 255 : sum_buff];
	}
}
