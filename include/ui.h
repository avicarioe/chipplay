/*
@file: ui.h
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

#ifndef UI_H__
#define UI_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "display.h"
#include "player.h"
#include "timeout.h"

#define UI_NAME_MAX     (DISPLAY_COLUMNS*3)
#define UI_NOTIFY_TIME  (2500)
#define UI_FPS_TIME     (100)

typedef struct ui_t {
	display_t* display;
	char name[UI_NAME_MAX + 1];
	uint8_t name_len;
	uint16_t duration;
	timeout_t notify_timer;
	timeout_t fps_timer;
	bool notify;
	int scroll_pos;
	bool scroll_enable;
} ui_t;

err_t ui_init(ui_t* self, display_t* display);
err_t ui_loadsong(ui_t* self, const char* name, const player_info_t* info);
void ui_setprogress(ui_t* self, uint16_t elapsed);
void ui_play(ui_t* self);
void ui_pause(ui_t* self);
void ui_volume(ui_t* self, uint8_t volume);
void ui_fire(ui_t* self);
void ui_notify(ui_t* self, const char* text);
void ui_hello(ui_t* self);

#endif // UI_H__
