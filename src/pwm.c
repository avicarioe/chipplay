#include "pwm.h"
#include "timer16.h"

#define TIMER_R   TIMER2_R //Do not change
#define OC_R      OC1_R

#define DEFAULT_DC 0

void pwm_init()
{
	OC_R->CON = 0;

	OC_R->RS = DEFAULT_DC;
	OC_R->R = DEFAULT_DC;

	OC_R->CONSET = PIC32_OCCON_PWM << PIC32_OCCON_CON_POS;
	OC_R->CONSET = PIC32_OCCON_ON;

	timer16_init(TIMER_R, PIC32_TPSB_1, 0x000000FFU);
	timer16_start(TIMER_R);
}

void pwm_set(uint8_t val)
{

	OC_R->RS = val;

}

