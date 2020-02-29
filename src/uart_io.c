/*
@file: uart_io.c
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

#include <stdio.h>
#include <uart.h>

/** Function prototypes *******************************************************/
static size_t write_uart1(FILE* instance, const char* bp, size_t n);
static size_t read_uart1(FILE* instance, char* bp, size_t n);

/** Global variables **********************************************************/
static struct File_methods _uart_file_methods = {
	.read = read_uart1,
	.write = write_uart1,
};

static FILE _std = {
	.vmt = &_uart_file_methods,
};

FILE* const stdin = &_std;
FILE* const stdout = &_std;
FILE* const stderr = &_std;

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions *************************************************************/


static size_t write_uart1(FILE* instance, const char* bp, size_t n)
{
	int i;
	for(i = 0; i < n; i++) {

		uart_tx(UART1_R, bp[i]);

	}

	return n;
}

static size_t read_uart1(FILE* instance, char* bp, size_t n)
{
	return 0;
}
