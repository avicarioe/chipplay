/*
@file: spi.c
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

#include "spi.h"

#define SPI_BRG_FS(fr, fs) ((fr)/2/(fs) + 1)

void spi_init(SPI_reg_t* reg)
{
	reg->CON = 0;
	reg->BUF = 0;

	reg->BRG = SPI_BRG_FS(FREQ_BUS, SPI_DEFAULT_FREQ);

	reg->STATCLR = PIC32_SPISTAT_SPIROV;
	reg->CONSET = PIC32_SPICON_MSTEN;
	reg->CONSET = PIC32_SPICON_CKP;
	reg->CONSET = PIC32_SPICON_ON;
}

void spi_set_freq(SPI_reg_t* reg, uint32_t freq)
{
	while(reg->STAT & PIC32_SPISTAT_SPIBUSY);

	reg->BRG = SPI_BRG_FS(FREQ_BUS, freq);
}

void spi_xfer(SPI_reg_t* reg, spi_xfer_t* xfer)
{
	uint16_t max = xfer->rx_len;
	if(xfer->tx_len > max) {
		max = xfer->tx_len;
	}

	for(int i = 0; i < max; i++) {
		uint8_t rx;
		uint8_t tx = 0xFF;

		if(i < xfer->tx_len) {
			tx = xfer->tx_buff[i];
		}

		spi_single(reg, tx, &rx);

		if(i < xfer->rx_len) {
			xfer->rx_buff[i] = rx;
		}
	}
}

void spi_single(SPI_reg_t* reg, uint8_t tx, uint8_t* rx)
{
	while(!(reg->STAT & PIC32_SPISTAT_SPITBE));

	reg->BUF = tx;

	while(!(reg->STAT & PIC32_SPISTAT_SPIRBF));

	*rx = reg->BUF;
}
