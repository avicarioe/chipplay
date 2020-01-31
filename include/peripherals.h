#ifndef __PERIPHERALS_H
#define __PERIPHERALS_H

#include <stdint.h>
#include <pic32mx.h>

#ifdef __cplusplus
#define __I     volatile
#else
#define __I     volatile const
#endif
#define __O     volatile
#define __IO    volatile

/** Base address **************************************************************/
#define PERIPHERALS_BASE  (0xBF800000)
#define UART1_BASE        (PERIPHERALS_BASE + 0x6000)
#define UART2_BASE        (PERIPHERALS_BASE + 0x6200)

/** Register structures *******************************************************/
typedef struct {
	__IO uint32_t MODE;
	__O  uint32_t MODECLR;
	__O  uint32_t MODESET;
	__O  uint32_t MODEINV;
	__IO uint32_t STA;
	__O  uint32_t STACLR;
	__O  uint32_t STASET;
	__O  uint32_t STAINV;
	__IO uint32_t TXREG;
	__I  uint32_t RESERVED0[3];
	__I  uint32_t RXREG;
	__I  uint32_t RESERVED1[3];
	__IO uint32_t BRG;
	__O  uint32_t BRGCLR;
	__O  uint32_t BRGSET;
	__O  uint32_t BRGINV;
} UART_reg_t;


/** Declaration ***************************************************************/
#define UART1_R                       ((UART_reg_t*) UART1_BASE)
#define UART2_R                       ((UART_reg_t*) UART2_BASE)

#endif //__PERIPHERALS_H
