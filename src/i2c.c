#include "i2c.h"

#define I2C_BRG_FS(fr, fs) ((fr)/(fs) - (fr / 10000000) - 1)

/** Global variables **********************************************************/
static volatile uint32_t count;

/** Function prototypes *******************************************************/
static err_t wait_idle(I2C_reg_t* reg);
static err_t send_start(I2C_reg_t* reg);
static err_t send_stop(I2C_reg_t* reg);
static err_t send_byte(I2C_reg_t* reg, uint8_t tx);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static err_t wait_idle(I2C_reg_t* reg)
{
	//TODO Add timeout

	uint32_t bits = 0;

	bits |= PIC32_I2CCON_SEN;
	bits |= PIC32_I2CCON_PEN;
	bits |= PIC32_I2CCON_RCEN;
	bits |= PIC32_I2CCON_RSEN;
	bits |= PIC32_I2CCON_ACKEN;


	while ((reg->CON & bits) ||
			reg->STAT & PIC32_I2CSTAT_TRSTAT);

	return SUCCESS;
}

static err_t send_start(I2C_reg_t* reg)
{
	RETURN_ON_ERROR(wait_idle(reg));

	reg->CONSET = PIC32_I2CCON_SEN;

	if (reg->STAT & PIC32_I2CSTAT_BCL) {
		return ERR_INTERNAL;
	}
	
	return wait_idle(reg);
}

static err_t send_stop(I2C_reg_t* reg)
{
	RETURN_ON_ERROR(wait_idle(reg));

	reg->CONSET = PIC32_I2CCON_PEN;

	//TODO timeout
	while (reg->CON & PIC32_I2CCON_PEN);

	reg->CONCLR = PIC32_I2CCON_RCEN;
	reg->STATCLR = PIC32_I2CSTAT_IWCOL;
	reg->STATCLR = PIC32_I2CSTAT_BCL;

	return wait_idle(reg);
}

static err_t send_byte(I2C_reg_t* reg, uint8_t tx)
{
	reg->TRN = tx;

	if (reg->STAT & PIC32_I2CSTAT_IWCOL) {
		return ERR_INTERNAL;
	}

	//TODO timeout
	while (reg->STAT & PIC32_I2CSTAT_TRSTAT);

	if (reg->STAT & PIC32_I2CSTAT_BCL) {
		return ERR_INTERNAL;
	}

	return wait_idle(reg);
}

/** Public functions **********************************************************/

err_t i2c_init(I2C_reg_t* reg, i2c_freq_t freq)
{
	reg->CON = 0;

	reg->BRG = I2C_BRG_FS(FREQ_BUS, freq);

	reg->STAT = 0;

	reg->CONSET = PIC32_I2CCON_ON;

	return SUCCESS;
}

err_t i2c_tx(I2C_reg_t* reg, uint8_t addr, uint8_t tx, const uint8_t* data,
		uint8_t len, bool no_stop)
{
	send_start(reg);

	send_byte(reg, (addr << 1) | 0);

	RETURN_ON_ERROR(wait_idle(reg));

	if (reg->STAT & PIC32_I2CSTAT_ACKSTAT) {
		return ERR_INTERNAL;
	}

	send_byte(reg, tx);
	if (reg->STAT & PIC32_I2CSTAT_ACKSTAT) {
		return ERR_INTERNAL;
	}

	for (int i = 0; i < len; i++) {
		send_byte(reg, data[i]);

		if (reg->STAT & PIC32_I2CSTAT_ACKSTAT) {
			return ERR_INTERNAL;
		}
	}

	if (!no_stop) {
		return send_stop(reg);
	}

	return SUCCESS;
}
