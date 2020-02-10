#include "uart.h"

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions *************************************************************/
void uart_init(UART_reg_t* reg, uint32_t baud)
{
	reg->MODECLR = 0xFFFFFFFF;
	reg->MODESET = PIC32_UMODE_ON;
	reg->STASET = PIC32_USTA_UTXEN;
	reg->BRG = PIC32_BRG_BAUD(FREQ_BUS, baud);
}

void uart_tx(UART_reg_t* reg, uint8_t txd)
{
	while(reg->STA & PIC32_USTA_UTXBF);
	reg->TXREG = txd;
}
