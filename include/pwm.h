#ifndef __PWM_H
#define __PWM_H

#include <pic32mx.h>
#include "peripherals.h"

void pwm_init();
void pwm_ch_init(OC_reg_t* reg);
void pwm_set(OC_reg_t* reg, uint8_t val);

#endif // __PWM_H
