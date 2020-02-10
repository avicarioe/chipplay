#include "timeout.h"
#include "timer16.h"
#include "interrupts.h"


/** Global variables **********************************************************/
static volatile uint32_t count;

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/
__ISR(TIMER3)
{
	IFSCLR(0) = PIC32_IFS_T3IF;
	count++;
	return;
}

/** Function definitions ******************************************************/

/** Public functions **********************************************************/

void timeout_init()
{
	count = 0;

	timer16_init(TIMER3_R, PIC32_TPSB_64, 1249);
	timer16_start(TIMER3_R);

	DISABLE_IRQ();

	IPCCLR(3) = PIC32_IPC_TxIP_MASK | PIC32_IPC_TxIS_MASK;
	IPCSET(3) = IRQ_PRIORITY_LOW << PIC32_IPC_TxIP_POS;
	IECSET(0) = PIC32_IEC_T3IE;

	ENABLE_IRQ();

}

void timeout_start(timeout_t* timeout, uint32_t ms)
{
	timeout->start = count;
	timeout->ms = ms;
}

bool timeout_check(timeout_t* timeout)
{
	uint32_t temp = count;
	return temp - timeout->start < timeout->ms;
}

void timeout_delay(uint32_t ms)
{
	timeout_t to;
	timeout_start(&to, ms);
	while(timeout_check(&to));
}

