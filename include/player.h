/*
@file: player.h
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

#ifndef PLAYER_H__
#define PLAYER_H__

#include <stdbool.h>
#include <stdint.h>
#include "peripherals.h"
#include "ff.h"
#include "util.h"

#define PLAYER_MAX_VOLUME  (10)

typedef enum {
	PLAYER_EVT_END,
	PLAYER_EVT_ERROR,
	PLAYER_EVT_FS_ERR,
	PLAYER_EVT_WV_ERR,
} player_evt_t;

typedef void (*player_cb_t)(FIL* fd, player_evt_t  evt);

typedef struct player_conf_t {
	OC_reg_t* left_pwm;
	OC_reg_t* right_pwm;
	player_cb_t cb;
} player_conf_t;

typedef enum {
	PLAYER_STA_PLAY,
	PLAYER_STA_STOP,
	PLAYER_STA_PAUSE,
	PLAYER_STA_UNLOAD,
} player_sta_t;

typedef struct player_info_t {
	uint16_t duration;
	uint16_t sample_rate;
	bool stereo;
	player_sta_t status;
} player_info_t;

void player_init(player_conf_t* conf);

err_t player_load(FIL* fd);

const player_info_t* player_get_info();

void player_play();
void player_pause();
void player_stop();

uint8_t player_progress();
uint16_t player_get_elapsed();

uint8_t player_volume_inc(int sign);

void player_fire();

#endif // PLAYER_H__
