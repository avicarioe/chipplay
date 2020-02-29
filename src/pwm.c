/*
@file: pwm.c
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

#include "pwm.h"
#include "timer16.h"

#define DEFAULT_DC 0

void pwm_init()
{
	timer16_init(TIMER2_R, PIC32_TPSB_1, 0x000000FFU);
	timer16_start(TIMER2_R);
}

void pwm_ch_init(OC_reg_t* reg)
{
	reg->CON = 0;

	reg->RS = DEFAULT_DC;
	reg->R = DEFAULT_DC;

	reg->CONSET = PIC32_OCCON_PWM << PIC32_OCCON_CON_POS;
	reg->CONSET = PIC32_OCCON_ON;
}

void pwm_set(OC_reg_t* reg, uint8_t val)
{

	reg->RS = val;

}

