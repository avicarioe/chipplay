/*
@file: timeout.h
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
