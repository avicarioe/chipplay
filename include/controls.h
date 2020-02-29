/*
@file: controls.h
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

#ifndef CONTROLS_H__
#define CONTROLS_H__

#include <stdbool.h>
#include <stdint.h>

#define CONTROLS_N_EVT (4)

typedef void (*controls_cb_t)(uint8_t evt);

void controls_init(controls_cb_t cb);

void controls_fire();

#endif // CONTROLS_H__
