/*
@file: uart.c
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

#include "uart.h"

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions *************************************************************/
void uart_init(UART_reg_t* reg, uint32_t baud)
{
	reg->MODECLR = 0xFFFFFFFF;
	reg->MODESET = PIC32_UMODE_ON;
	reg->STASET = PIC32_USTA_UTXEN;
	reg->BRG = PIC32_BRG_BAUD(FREQ_BUS, baud);
}

void uart_tx(UART_reg_t* reg, uint8_t txd)
{
	while(reg->STA & PIC32_USTA_UTXBF);
	reg->TXREG = txd;
}
