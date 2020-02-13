#include "timer16.h"
#include "util.h"

static const uint16_t ps_lu[] = {1, 2, 4, 8, 16, 32, 64, 256};

void timer16_init(TIMER_reg_t* reg, pic32_tpsb_t ps, uint16_t period)
{
	ASSERT(reg != TIMER1_R);

	reg->CONCLR = 0xFFFFFFFFU;
	reg->TMRCLR = 0xFFFFFFFFU;

	reg->CONSET = ps << PIC32_TCON_TCKPS_POS;
	reg->PR = period;
}

void timer16_set_freq(TIMER_reg_t* reg, uint32_t freq)
{
	uint8_t ps_reg = (reg->CON >> PIC32_TCON_TCKPS_POS);
	ps_reg &= PIC32_TCON_TCKPS_MASK;
	uint16_t ps = ps_lu[ps_reg];

	uint32_t pr = FREQ_BUS/ps/freq - 1;

	reg->PR = pr;
}

void timer16_start(TIMER_reg_t* reg)
{
	reg->TMRCLR = 0xFFFFFFFFU;
	reg->CONSET = PIC32_TCON_ON;
}

void timer16_stop(TIMER_reg_t* reg)
{
	reg->CONCLR = PIC32_TCON_ON;
}

void timer16_set_pr(TIMER_reg_t* reg, uint16_t period)
{
	reg->PR = period;
}
