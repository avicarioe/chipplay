/*
@file: timeout.c
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

#include "timeout.h"
#include "timer16.h"
#include "interrupts.h"


/** Global variables **********************************************************/
static volatile uint32_t count;

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/
__ISR(TIMER3)
{
	IFSCLR(0) = PIC32_IFS_T3IF;
	count++;
	return;
}

/** Function definitions ******************************************************/

/** Public functions **********************************************************/

void timeout_init()
{
	count = 0;

	timer16_init(TIMER3_R, PIC32_TPSB_64, 124);
	timer16_start(TIMER3_R);

	TIMER16_ENABLE_INT(3, IRQ_PRIORITY_LOW);
}

void timeout_start(timeout_t* timeout, uint32_t ms)
{
	timeout->start = count / 10;
	timeout->ms = ms;
}

bool timeout_check(timeout_t* timeout)
{
	uint32_t temp = count / 10;
	return temp - timeout->start < timeout->ms;
}

void timeout_delay(uint32_t ms)
{
	timeout_t to;
	timeout_start(&to, ms);
	while(timeout_check(&to));
}

uint32_t timeout_count()
{
	return count;
}
