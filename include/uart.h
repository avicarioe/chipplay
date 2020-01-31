#ifndef __UART_H
#define __UART_H

#include <pic32mx.h>
#include "peripherals.h"


void uart_init(UART_reg_t* reg, uint32_t baud);
void uart_tx(UART_reg_t* reg, uint8_t txd);

#endif // __UART_H
