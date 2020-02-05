#ifndef __TIMER16_H
#define __TIMER16_H

#include <pic32mx.h>
#include "peripherals.h"

void timer16_init(TIMER_reg_t* reg, pic32_tpsb_t ps, uint16_t period);
void timer16_start(TIMER_reg_t* reg);
void timer16_stop(TIMER_reg_t* reg);

#endif // __TIMER16_H
