/*
@file: spi.h
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

#ifndef __SPI_H
#define __SPI_H

#include <pic32mx.h>
#include "peripherals.h"

#define SPI_DEFAULT_FREQ 100000U

typedef struct spi_xfer_t {
	const uint8_t* tx_buff;
	uint16_t tx_len;
	uint8_t* rx_buff;
	uint16_t rx_len;
} spi_xfer_t;

void spi_init(SPI_reg_t* reg);
void spi_set_freq(SPI_reg_t* reg, uint32_t freq);
void spi_xfer(SPI_reg_t* reg, spi_xfer_t* xfer);
void spi_single(SPI_reg_t* reg, uint8_t tx, uint8_t* rx);

#endif // __SPI_H
