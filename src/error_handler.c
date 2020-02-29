/*
@file: error_handler.c
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

#include "log.h"
#include "main.h"
#include "util.h"
#include <stdint.h>

#define ERROR_DISPLAY

void error_fault_handler(err_t err, uint32_t line, const char* file)
{
	// TODO Disable irq
	LOG_ERROR("Fatal error %d at %s:%u", err, file, (unsigned int)line);

#ifndef DEBUG
	LOG_WARN("System reset");
	// TODO System reset
	for(;;);
#else
#ifdef ERROR_DISPLAY

	display_clear(&display);
	display_drawtext(&display, "Error!", 2, 5);
	display_drawtext(&display, "Please, reboot", 4, 1);
	display_drawtext(&display, "the device.", 5, 3);

#endif
	for(;;);
#endif // DEBUG
}
