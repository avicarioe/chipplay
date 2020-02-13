#ifndef __TIMER16_H
#define __TIMER16_H

#include <pic32mx.h>
#include "peripherals.h"

#define TIMER16_ENABLE_INT(num, prio)                        \
	DISABLE_IRQ();                                           \
	IPCCLR(num) = PIC32_IPC_TxIP_MASK | PIC32_IPC_TxIS_MASK; \
	IPCSET(num) = (prio) << PIC32_IPC_TxIP_POS;              \
	IECSET(0) = PIC32_IEC_T ## num ## IE;                    \
	ENABLE_IRQ();

void timer16_init(TIMER_reg_t* reg, pic32_tpsb_t ps, uint16_t period);
void timer16_set_freq(TIMER_reg_t* reg, uint32_t freq);
void timer16_start(TIMER_reg_t* reg);
void timer16_stop(TIMER_reg_t* reg);
void timer16_set_pr(TIMER_reg_t* reg, uint16_t period);

#endif // __TIMER16_H
