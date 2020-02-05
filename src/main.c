#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "peripherals.h"
#include "uart.h"
#include "log.h"
#include "version.h"
#include "timer16.h"
#include "util.h"
#include "interrupts.h"

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static void clock_initialization();

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static void clock_initialization()
{
	/* Set the peripheral bus clock to the same frequency as the sysclock */
	SYSKEY = 0xAA996655;        /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;        /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000;       /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;               /* Lock OSCCON */
}

/** Main function *************************************************************/
int main(void)
{

	clock_initialization();
	uart_init(UART1_R, 115200);

	LOG_INFO(VERSION_INFO);

	LOG_DEBUG("Debug %s:%d", __FILE__, __LINE__);
	LOG_INFO("Info %s:%d", __FILE__, __LINE__);
	LOG_WARN("Warning %s:%d", __FILE__, __LINE__);
	LOG_ERROR("Error %s:%d", __FILE__, __LINE__);

	timer16_init(TIMER2_R, PIC32_TPSB_256, 0x0000FFFFU);
	timer16_start(TIMER2_R);

	DISABLE_IRQ();

	INTCONSET = PIC32_INTCON_MVEC;

	IPCCLR(2) = PIC32_IPC_TxIP_MASK | PIC32_IPC_TxIS_MASK;
	IPCSET(2) = IRQ_PRIORITY_LOW << PIC32_IPC_TxIP_POS;
	IECSET(0) = PIC32_IEC_T2IE;

	ENABLE_IRQ();

	for(;;) {
		//LOG_INFO("Timer %u", (unsigned int)TIMER2_R->TMR);
	}

	return 0;
}

__ISR(TIMER2)
{
	IFSCLR(0) = PIC32_IFS_T2IF;
	LOG_INFO("IRQ");
	return;
}

