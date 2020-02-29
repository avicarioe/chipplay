/*
@file: display.h
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

#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "i2c.h"

#define DISPLAY_ADDR_1    (0x3C)
#define DISPLAY_ADDR_2    (0x3D)

#define DISPLAY_FONT_SIZE (96)
#define DISPLAY_FONT_MIN  (' ')
#define DISPLAY_LINES     (8)
#define DISPLAY_COLUMNS   (16)
#define DISPLAY_WIDTH     (DISPLAY_COLUMNS*8)

typedef struct display_t {
	I2C_reg_t* i2c;
	uint8_t addr;
} display_t;

typedef struct display_conf_t {
	I2C_reg_t* i2c;
	uint8_t addr;
} display_conf_t;

typedef struct display_rect_t {
	uint8_t line;
	uint8_t x;
	uint8_t width;
} display_rect_t;

err_t display_init(display_t* self, const display_conf_t* conf);

void display_drawrect(display_t* self, const display_rect_t* rect,
	const uint8_t* data);

void display_drawtext(display_t* self, const char* text, uint8_t line,
	uint8_t pos);

void display_drawtext_x(display_t* self, const char* text, uint8_t line,
	int8_t x);

void display_drawicon(display_t* self, uint8_t line, uint8_t x,
	const uint8_t* data);

void display_clear(display_t* self);

void display_clear_line(display_t* self, uint8_t line);

#endif // DISPLAY_H__
