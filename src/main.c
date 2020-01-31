#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "peripherals.h"
#include "uart.h"
#include "log.h"
#include "version.h"

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

	for(;;) {
	}

	return 0;
}
