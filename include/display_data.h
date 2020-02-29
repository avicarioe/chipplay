/*
@file: display_data.h
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

#ifndef DISPLAY_DATA_H__
#define DISPLAY_DATA_H__

#include <stdint.h>

#define DISPLAY_FONT_SIZE (96)
#define DISPLAY_FONT_MIN  (' ')

extern const uint8_t display_font[DISPLAY_FONT_SIZE * 8];

extern const uint8_t sym_left[8];
extern const uint8_t sym_right[8];
extern const uint8_t sym_ltail[8];
extern const uint8_t sym_rtail[8];
extern const uint8_t sym_play[8];
extern const uint8_t sym_resume[8];
extern const uint8_t sym_pause[8];
extern const uint8_t sym_stop[8];

const uint8_t* sym_get(char c);

#endif // DISPLAY_DATA_H__
