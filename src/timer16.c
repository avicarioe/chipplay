#include "timer16.h"
#include "util.h"

void timer16_init(TIMER_reg_t* reg, pic32_tpsb_t ps, uint16_t period)
{
	ASSERT(reg != TIMER1_R);

	reg->CONCLR = 0xFFFFFFFFU;
	reg->TMRCLR = 0xFFFFFFFFU;

	reg->CONSET = ps << PIC32_TCON_TCKPS_POS;
	reg->PR = period;
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
