/*
@file: timer16.h
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

#ifndef __TIMER16_H
#define __TIMER16_H

#include <pic32mx.h>
#include "peripherals.h"

#define TIMER16_ENABLE_INT(num, prio)                        \
	DISABLE_IRQ();                                           \
	IPCCLR(num) = PIC32_IPC_TxIP_MASK | PIC32_IPC_TxIS_MASK; \
	IPCSET(num) = (prio) << PIC32_IPC_TxIP_POS;              \
	IECSET(0) = PIC32_IEC_T ## num ## IE;                    \
	ENABLE_IRQ();

void timer16_init(TIMER_reg_t* reg, pic32_tpsb_t ps, uint16_t period);
void timer16_set_freq(TIMER_reg_t* reg, uint32_t freq);
void timer16_start(TIMER_reg_t* reg);
void timer16_stop(TIMER_reg_t* reg);
void timer16_set_pr(TIMER_reg_t* reg, uint16_t period);

#endif // __TIMER16_H
