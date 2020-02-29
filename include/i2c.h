/*
@file: i2c.h
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

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <pic32mx.h>
#include "peripherals.h"
#include "util.h"

typedef enum {
	I2C_FREQ_100K = 100000,
	I2C_FREQ_400K = 400000,
	I2C_FREQ_1M = 1000000,
} i2c_freq_t;

err_t i2c_init(I2C_reg_t* reg, i2c_freq_t freq);
err_t i2c_tx(I2C_reg_t* reg, uint8_t addr, uint8_t tx, const uint8_t* data,
	uint8_t len, bool no_stop);
err_t i2c_rx(I2C_reg_t* reg, uint8_t addr, uint8_t* data, uint8_t len);

#endif // __I2C_H
