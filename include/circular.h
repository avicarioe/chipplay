/*
@file: circular.h
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
