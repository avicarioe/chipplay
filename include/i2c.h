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
