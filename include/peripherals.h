#ifndef __PERIPHERALS_H
#define __PERIPHERALS_H

#include <stdint.h>
#include <pic32mx.h>

#define FREQ_BUS 80000000UL

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
#define TIMER1_BASE       (PERIPHERALS_BASE + 0x0600)
#define TIMER2_BASE       (PERIPHERALS_BASE + 0x0800)
#define TIMER3_BASE       (PERIPHERALS_BASE + 0x0a00)
#define TIMER4_BASE       (PERIPHERALS_BASE + 0x0c00)
#define TIMER5_BASE       (PERIPHERALS_BASE + 0x0e00)
#define SPI1_BASE         (PERIPHERALS_BASE + 0x5800)
#define SPI2_BASE         (PERIPHERALS_BASE + 0x5A00)

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

typedef struct {
	__IO uint32_t CON;
	__O  uint32_t CONCLR;
	__O  uint32_t CONSET;
	__O  uint32_t CONINV;
	__IO uint32_t TMR;
	__O  uint32_t TMRCLR;
	__O  uint32_t TMRSET;
	__O  uint32_t TMRINV;
	__IO uint32_t PR;
	__O  uint32_t PRCLR;
	__O  uint32_t PRSET;
	__O  uint32_t PRINV;
} TIMER_reg_t;

typedef struct {
	__IO uint32_t CON;
	__O  uint32_t CONCLR;
	__O  uint32_t CONSET;
	__O  uint32_t CONINV;
	__IO uint32_t STAT;
	__O  uint32_t STATCLR;
	__O  uint32_t STATSET;
	__O  uint32_t STATINV;
	__IO uint32_t BUF;
	__I  uint32_t RESERVED0[3];
	__IO uint32_t BRG;
	__O  uint32_t BRGCLR;
	__O  uint32_t BRGSET;
	__O  uint32_t BRGINV;
} SPI_reg_t;


/** Declaration ***************************************************************/
#define UART1_R                       ((UART_reg_t*) UART1_BASE)
#define UART2_R                       ((UART_reg_t*) UART2_BASE)
#define TIMER1_R                      ((TIMER_reg_t*) TIMER1_BASE)
#define TIMER2_R                      ((TIMER_reg_t*) TIMER2_BASE)
#define TIMER3_R                      ((TIMER_reg_t*) TIMER3_BASE)
#define TIMER4_R                      ((TIMER_reg_t*) TIMER4_BASE)
#define TIMER5_R                      ((TIMER_reg_t*) TIMER5_BASE)
#define SPI1_R                        ((SPI_reg_t*) SPI1_BASE)
#define SPI2_R                        ((SPI_reg_t*) SPI2_BASE)

/** Registers *****************************************************************/
#define PIC32_TCON_ON                  (1 << 15)
#define PIC32_TCON_FRZ                 (1 << 14)
#define PIC32_TCON_SIDL                (1 << 13)
#define PIC32_TCON_TGATE               (1 << 7)
#define PIC32_TCON_TCKPS_POS           (4)
#define PIC32_TCON_T32                 (1 << 3)
#define PIC32_TCON_TCS                 (1 << 1)

#define PIC33_IEC_T1IE                 (1 << 4)
#define PIC32_IEC_T2IE                 (1 << 8)
#define PIC32_IEC_T3IE                 (1 << 12)
#define PIC32_IEC_T4IE                 (1 << 16)
#define PIC32_IEC_T5IE                 (1 << 20)

#define PIC33_IFS_T1IF                 (1 << 4)
#define PIC32_IFS_T2IF                 (1 << 8)
#define PIC32_IFS_T3IF                 (1 << 12)
#define PIC32_IFS_T4IF                 (1 << 16)
#define PIC32_IFS_T5IF                 (1 << 20)

#define PIC32_IPC_TxIP_POS             (2)
#define PIC32_IPC_TxIP_MASK            (0b111 << 2)
#define PIC32_IPC_TxIS_POS             (0)
#define PIC32_IPC_TxIS_MASK            (0b11  << 0)

typedef enum {
	PIC32_TPSA_256 = 0b11U,
	PIC32_TPSA_64  = 0b10U,
	PIC32_TPSA_8   = 0b01U,
	PIC32_TPSA_1   = 0b00U,
} pic32_tpsa_t;

typedef enum {
	PIC32_TPSB_256 = 0b111U,
	PIC32_TPSB_64  = 0b110U,
	PIC32_TPSB_32  = 0b101U,
	PIC32_TPSB_16  = 0b100U,
	PIC32_TPSB_8   = 0b011U,
	PIC32_TPSB_4   = 0b010U,
	PIC32_TPSB_2   = 0b001U,
	PIC32_TPSB_1   = 0b000U,
} pic32_tpsb_t;


#endif //__PERIPHERALS_H
