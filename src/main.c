#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "peripherals.h"
#include "uart.h"
#include "log.h"
#include "version.h"
#include "util.h"
#include "timeout.h"
#include "mmc.h"
#include "ff.h"
#include "pwm.h"
#include "interrupts.h"
#include "player.h"

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static void clock_initialization();
static void player_cb(player_evt_t evt);
static void show_progress();

/** Callback definitions ******************************************************/
static void player_cb(player_evt_t evt)
{
	switch(evt) {
	case PLAYER_EVT_END:
		LOG_INFO("Play end success");
		break;
	default:
		LOG_WARN("Play error: %d", evt);
	}
}

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

static void show_progress()
{
	static timeout_t time;

	if(!timeout_check(&time)) {
		timeout_start(&time, 1000);

		LOG_INFO("Progress: %d", player_get_elapsed());

	}
}

/** Main function *************************************************************/
int main(void)
{

	clock_initialization();
	uart_init(UART1_R, 115200);

	INTERRUPTS_SET_MVEC();

	LOG_INFO(VERSION_INFO);

	timeout_init();
	pwm_init();

	player_conf_t conf;
	conf.cb = player_cb;
	conf.left_pwm = OC1_R;
	conf.right_pwm = OC2_R;
	player_init(&conf);

	mmc_init();

	FRESULT fr;
	FATFS fs;

	fr = f_mount(&fs, "", 0);

	LOG_DEBUG("Mount: %d", fr);

	FIL fil;
	fr = f_open(&fil, "senfull.wav", FA_READ);

	LOG_DEBUG("Open: %d", fr);

	ERROR_CHECK(player_load(&fil));

	const player_info_t* info = player_get_info();
	LOG_INFO("Play start: %d", info->duration);

	player_fire();
	player_fire();
	player_fire();
	player_fire();
	player_fire();

	player_play();

	for(;;) {
		player_fire();
		show_progress();
	}

	return 0;
}

