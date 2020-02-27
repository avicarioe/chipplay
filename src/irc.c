#include "irc.h"
#include "peripherals.h"
#include "interrupts.h"
#include "timeout.h"
#include "util.h"

#define EXT_INT_BIT (1 << 7)
#define EXT_INT_EP  PIC32_INTCON_INT1EP
#define INT_PORT    GPIOD_R
#define INT_PIN     (8)
#define EXT_INT_IPC (1)
#define IPC_IP_POS  (26)
#define IPC_IS_POS  (24)
#define IPC_IP_MASK (0x07 << IPC_IP_POS)
#define IPC_IS_MASK (0x03 << IPC_IS_POS)
#define INT_PRIO    IRQ_PRIORITY_HIGH

/** Global variables **********************************************************/
static irc_cb_t s_cb;
static volatile uint32_t last_time;
static volatile uint8_t times[34];
static volatile uint8_t pos;

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/
__ISR(EXT1)
{
	IFSCLR(0) = EXT_INT_BIT;

	if (pos < sizeof(times)) {
		uint32_t time_temp = timeout_count();
		times[pos++] = time_temp - last_time;
		last_time = time_temp;
	}
	

	return;
}

/** Function definitions ******************************************************/

/** Public functions **********************************************************/
void irc_init(irc_cb_t cb)
{
	ASSERT(cb != NULL);
	s_cb = cb;

	last_time = 0;
	pos = 0;

	INT_PORT->TRISSET = (1 << INT_PIN);

	DISABLE_IRQ();
	IECCLR(0) = EXT_INT_BIT;
	INTCONCLR = EXT_INT_EP;
	IPCCLR(EXT_INT_IPC) = IPC_IP_MASK | IPC_IS_MASK;
	IFSCLR(0) = EXT_INT_BIT;
	IPCSET(EXT_INT_IPC) = INT_PRIO << IPC_IP_POS;
	IECSET(0) = EXT_INT_BIT;
	ENABLE_IRQ();

}

#include "log.h"

void irc_fire()
{
	if (pos >= sizeof(times)) {
		for (int i = 0; i < sizeof(times); i++) {
			LOG_INFO("Time %d: %d: %d", i, times[i], times[i] > 18);
		}
		s_cb(0);
		pos = 0;
	}
}
