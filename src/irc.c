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

#define AGC_TH_MIN  (120)
#define AGC_TH_MAX  (150)
#define REPEAT_TH   (90)
#define SYM_TH      (18)
#define SYM_TH_MAX  (40)

#define N_BITS      (32)

typedef enum {
	STATE_WAIT,
	STATE_AGC,
	STATE_DATA,
	STATE_NEW,
} state_t;

/** Global variables **********************************************************/
static irc_cb_t s_cb;
static volatile uint8_t times[N_BITS];
static volatile uint8_t n_bit;

static volatile state_t state;

/** Function prototypes *******************************************************/
static uint8_t dec_byte(uint8_t* t);

/** Callback definitions ******************************************************/
__ISR(EXT1)
{
	static uint32_t last_time = 0;

	IFSCLR(0) = EXT_INT_BIT;

	uint32_t time_temp = timeout_count();
	uint32_t diff = time_temp - last_time;
	last_time = time_temp;

	//LOG_INFO("agc %d %d", (unsigned int)diff, state);

	switch (state) {
	case STATE_WAIT:
		state = STATE_AGC;

		break;
	case STATE_AGC:
		if (diff > AGC_TH_MAX) {
			state = STATE_AGC;
		} else if (diff > AGC_TH_MIN) {
			state = STATE_DATA;
		} else {
			state = STATE_WAIT;
		}

		break;
	case STATE_DATA:
		if (diff > SYM_TH_MAX) {
			state = STATE_AGC;
			n_bit = 0;

		} else {
			if (n_bit < N_BITS) {
				times[n_bit] = diff;
			}

			n_bit++;
			if (n_bit == N_BITS) {
				state = STATE_NEW;
			}
		}

	case STATE_NEW:

		break;
	default:
		break;
	};

	return;
}

/** Function definitions ******************************************************/
static uint8_t dec_byte(uint8_t* t)
{
	uint8_t byte = 0;

	for (int i = 0; i < 8; i++) {
		uint8_t sym = (t[i] > SYM_TH)? 1: 0;

		byte |= sym << i;
	}
	return byte;
}

/** Public functions **********************************************************/
void irc_init(irc_cb_t cb)
{
	ASSERT(cb != NULL);
	s_cb = cb;

	n_bit = 0;
	state = STATE_WAIT;

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

void irc_fire()
{
	if (state == STATE_NEW) {
		ASSERT(n_bit == N_BITS);

		uint8_t t[N_BITS];

		for (int i = 0; i < N_BITS; i++) {
			t[i] = times[i];
		}

		state = STATE_WAIT;
		n_bit = 0;

		uint8_t bytes[4];
		for (int i = 0; i < sizeof(bytes); i++) {
			bytes[i] = dec_byte(t + i*8);
		}

		uint16_t cmd = (bytes[0] << 8) | bytes[2];
		uint16_t check = (bytes[1] << 8) | bytes[3];
		check = ~check;

		if (cmd == check) {
			s_cb(cmd);
		}
	}
}
