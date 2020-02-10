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
