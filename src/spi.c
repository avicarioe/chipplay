#include "spi.h"

void spi_init(SPI_reg_t* reg)
{
	reg->CONCLR = 0xFFFFFFFF;
	reg->BUF = 0;

	reg->BRG = PIC32_BRG_BAUD(FREQ_BUS, SPI_DEFAULT_FREQ);

	reg->STATCLR = PIC32_SPISTAT_SPIROV;
	reg->CONSET = PIC32_SPICON_MSTEN;
	reg->CONSET = PIC32_SPICON_ON;

}

void spi_set_freq(SPI_reg_t* reg, uint32_t freq)
{
	while(reg->STAT & PIC32_SPISTAT_SPIBUSY);

	reg->BRG = PIC32_BRG_BAUD(FREQ_BUS, freq);
}

void spi_xfer(SPI_reg_t* reg, spi_xfer_t* xfer)
{
	uint16_t max = xfer->rx_len;
	if (xfer->tx_len > max) {
		max = xfer->tx_len;
	}

	for (int i = 0; i < max; i++) {
		uint8_t rx;
		uint8_t tx = 0xFF;

		if (i < xfer->tx_len) {
			tx = xfer->tx_buff[i];
		}

		spi_single(reg, tx, &rx);

		if (i < xfer->rx_len) {
			xfer->rx_buff[i] = rx;
		}
	}
}

void spi_single(SPI_reg_t* reg, uint8_t tx, uint8_t* rx)
{
	while(!(reg->STAT & PIC32_SPISTAT_SPITBE));

	reg->BUF = tx;
	
	while(!(reg->STAT & PIC32_SPISTAT_SPITBF));

	*rx = reg->BUF;
}
