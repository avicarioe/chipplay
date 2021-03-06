/*
@file: player.c
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

#define LOG_MODULE_NAME "player"

#include "main.h"
#include "player.h"
#include "circular.h"
#include "wave.h"
#include "pwm.h"
#include "timer16.h"
#include "interrupts.h"
#include "log.h"

#define TIMER_R TIMER4_R

typedef struct player_t {
	OC_reg_t* left_pwm;
	OC_reg_t* right_pwm;
	player_cb_t cb;
	circular_t pcm;
	circular_t sd;
	wave_t wave;
	FIL* fd;
	player_info_t info;
	uint8_t volume;
} player_t;


/** Global variables **********************************************************/
player_t player;
static uint8_t buffer[4056];
static uint8_t sd_data[8192];
static uint8_t pcm_data[8192];
static volatile uint32_t count;

static const uint8_t volume_lut[PLAYER_MAX_VOLUME + 1] = {
	0, 4, 6, 9, 13, 18, 25, 35, 50, 71, 100
};
static const uint8_t volume_offset_lut[PLAYER_MAX_VOLUME + 1] = {
	128, 123, 120, 116, 111, 105, 96, 83, 64, 37, 0
};


/** Function prototypes *******************************************************/
static void read_sd();
static void dec_wave();

/** Callback definitions ******************************************************/
__ISR(TIMER4)
{
	IFSCLR(0) = PIC32_IFS_T4IF;

	uint8_t volume = volume_lut[player.volume];
	uint8_t off = volume_offset_lut[player.volume];

	uint16_t left = circular_get(&player.pcm);
	uint16_t right = circular_get(&player.pcm);
	left = (left * volume) / 100 + off;
	right = (right * volume) / 100 + off;
	pwm_set(player.left_pwm, left);
	pwm_set(player.right_pwm, right);

	count++;

	return;
}

/** Function definitions ******************************************************/
static void read_sd()
{
	if(player.fd == NULL) {
		return;
	}

	if(circular_free(&player.sd) >= sizeof(buffer)) {
		FRESULT fr;
		UINT br;
		fr = f_read(player.fd, buffer, sizeof(buffer), &br);

		if(fr != FR_OK) {
			LOG_WARN("Read err %d", fr);
			player_stop();
			player.cb(player.fd, PLAYER_EVT_FS_ERR);
			return;
		}

		if(br != sizeof(buffer)) {
			player.fd = NULL;
		}

		circular_write(&player.sd, buffer, br);
	}
}

static void dec_wave()
{
	if(circular_used(&player.sd) < player.wave.align &&
		player.fd != NULL) {
		return;
	}

	if(circular_used(&player.pcm) <= sizeof(pcm_data) / 2) {
		uint32_t samples = wave_dec(&player.wave);
		(void)samples;

		if(samples == 0) {
			player_stop();
			player.cb(player.fd, PLAYER_EVT_WV_ERR);
			return;
		}
	}
}

/** Public functions **********************************************************/
void player_init(player_conf_t* conf)
{
	player.left_pwm = conf->left_pwm;
	player.right_pwm = conf->right_pwm;
	player.cb = conf->cb;
	player.info.status = PLAYER_STA_UNLOAD;
	player.volume = PLAYER_MAX_VOLUME;

	pwm_ch_init(player.left_pwm);
	pwm_ch_init(player.right_pwm);

	circular_init(&player.pcm, pcm_data, sizeof(pcm_data));
	circular_init(&player.sd, sd_data, sizeof(sd_data));

	timer16_init(TIMER_R, PIC32_TPSB_4, 452);
	TIMER16_ENABLE_INT(4, IRQ_PRIORITY_LOW);
}

err_t player_load(FIL* fd)
{
	err_t ret;
	FRESULT fr;
	UINT br;

	player.fd = fd;

	fr = f_read(player.fd, buffer, sizeof(buffer), &br);
	circular_write(&player.sd, buffer, br);

	LOG_DEBUG("Read: %d, n: %d", fr, br);

	if(fr != FR_OK) {
		return ERR_INTERNAL;
	}

	ret = wave_init(&player.wave, &player.sd, &player.pcm);

	LOG_DEBUG("Wave: %d", ret);

	RETURN_ON_ERROR(ret);

	LOG_DEBUG("Info: L: %d, ch: %d, sr: %d, br: %d, al: %d, bps: %d",
		(unsigned int) player.wave.length,
		player.wave.channels,
		(unsigned int) player.wave.sample_rate,
		(unsigned int) player.wave.byte_rate,
		player.wave.align, player.wave.bpsample);

	player.info.duration = player.wave.length / player.wave.byte_rate;
	player.info.sample_rate = player.wave.sample_rate;
	player.info.stereo = player.wave.channels > 1;
	player.info.status = PLAYER_STA_STOP;
	count = 0;

	return SUCCESS;
}

const player_info_t* player_get_info()
{
	return &player.info;
}

void player_play()
{
	timer16_set_freq(TIMER_R, player.wave.sample_rate);
	timer16_start(TIMER_R);
	player.info.status = PLAYER_STA_PLAY;
}

void player_pause()
{
	timer16_stop(TIMER_R);
	player.info.status = PLAYER_STA_PAUSE;
}

void player_stop()
{
	player_pause();
	player.fd = NULL;
	player.wave.length = 0;
	player.wave.position = 0;
	player.info.duration = 0;
	player.info.status = PLAYER_STA_UNLOAD;
	circular_clear(&player.sd);
	circular_clear(&player.pcm);
	count = 0;
}

uint8_t player_volume_inc(int sign)
{
	if(sign > 0 && player.volume < PLAYER_MAX_VOLUME) {
		player.volume++;
	} else if(sign < 0 && player.volume > 0) {
		player.volume--;
	}

	return player.volume;
}

uint8_t player_progress()
{
	return wave_progress(&player.wave);
}

uint16_t player_get_elapsed()
{
	return count / player.info.sample_rate;
}

void player_fire()
{
	if(player.fd == NULL && player.info.duration == 0) {
		return;

	} else if(!wave_end(&player.wave)) {
		dec_wave();
		read_sd();

	} else if(!circular_used(&player.pcm)) {
		player_stop();
		player.cb(player.fd, PLAYER_EVT_END);
	}
}
