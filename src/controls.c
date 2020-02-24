#include "controls.h"
#include "peripherals.h"
#include "util.h"


/** Global variables **********************************************************/
static controls_cb_t s_cb;
static uint8_t last_flags;

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions **********************************************************/
void controls_init(controls_cb_t cb)
{
	ASSERT(cb != NULL);
	s_cb = cb;

	GPIOF_R->TRISSET = (1 << 1);
	GPIOD_R->TRISSET = (1 << 7) | (1 << 6) | (1 << 5);
}

void controls_fire()
{
	uint8_t flags = 0;
	flags |= ((GPIOD_R->PORT & (1 << 7)) != 0) << 0;
	flags |= ((GPIOD_R->PORT & (1 << 6)) != 0) << 1;
	flags |= ((GPIOD_R->PORT & (1 << 5)) != 0) << 2;
	flags |= ((GPIOF_R->PORT & (1 << 1)) != 0) << 3;


	for (int i = 0; i < CONTROLS_N_EVT; i++) {
		if (flags & (1 << i) && !(last_flags & (1 << i))) {
			s_cb(i);
		}
	}

	last_flags = flags;

}
