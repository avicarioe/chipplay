#include "pwm.h"
#include "timer16.h"

#define DEFAULT_DC 0

void pwm_init()
{
	timer16_init(TIMER2_R, PIC32_TPSB_1, 0x000000FFU);
	timer16_start(TIMER2_R);
}

void pwm_ch_init(OC_reg_t* reg)
{
	reg->CON = 0;

	reg->RS = DEFAULT_DC;
	reg->R = DEFAULT_DC;

	reg->CONSET = PIC32_OCCON_PWM << PIC32_OCCON_CON_POS;
	reg->CONSET = PIC32_OCCON_ON;
}

void pwm_set(OC_reg_t* reg, uint8_t val)
{

	reg->RS = val;

}

