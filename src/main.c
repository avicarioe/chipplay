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
#include "controls.h"
#include "display.h"
#include "i2c.h"
#include "ui.h"

#define MAX_FILES (30)
#define MAX_LEN   (40)

/** Global variables **********************************************************/
static char files[MAX_FILES*MAX_LEN];
static uint8_t n_files;
static uint8_t p_file;
static FIL fil;
static display_t display;
static ui_t ui;

/** Function prototypes *******************************************************/
static void clock_initialization();
static void player_cb(FIL* fd, player_evt_t evt);
static void show_progress();
static void ls_wav(DIR* dp, char* names, uint8_t len_name, uint8_t* n_names);
static void controls_cb(uint8_t evt);
static void load_file(const char* filename);
static void load_next(int sign);

/** Callback definitions ******************************************************/
static void controls_cb(uint8_t evt)
{
	LOG_INFO("Controls %d", evt);

	const player_info_t* info = player_get_info();

	switch (evt) {
	case 0:
		if (info->status == PLAYER_STA_STOP) {
			load_next(-1);
		} else if (info->status == PLAYER_STA_PLAY) {
			uint8_t vol = player_volume_inc(-1);
			LOG_INFO("Vol %d", vol);
		}

		break;
	case 1:
		if (info->status == PLAYER_STA_STOP ||
				info->status == PLAYER_STA_PAUSE) {
			LOG_INFO("Playing");
			player_play();
		} else if (info->status == PLAYER_STA_PLAY) {
			LOG_INFO("Pause");
			player_pause();
		}

		break;
	case 2:
		LOG_INFO("Stop");
		load_file(files + p_file*MAX_LEN);

		break;
	case 3:
		if (info->status == PLAYER_STA_STOP) {
			load_next(1);
		} else if (info->status == PLAYER_STA_PLAY) {
			uint8_t vol = player_volume_inc(1);
			LOG_INFO("Vol %d", vol);
		}

		break;
	default:
		LOG_WARN("Invalid control");
		break;
	}
}

static void player_cb(FIL* fd, player_evt_t evt)
{
	switch(evt) {
	case PLAYER_EVT_END:
		LOG_INFO("Play end success");
		break;
	default:
		LOG_WARN("Play error: %d", evt);
	}

	load_next(1);
	player_play();
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

		const player_info_t* info = player_get_info();
		if (info->status != PLAYER_STA_PLAY) {
			return;
		}

		LOG_INFO("Progress: %d", player_get_elapsed());

	}
}

static void ls_wav(DIR* dp, char* names, uint8_t len_name, uint8_t* n_names)
{
	int i;
	for (i = 0; i < *n_names; i++) {
		FILINFO fno;
		FRESULT fr;
		fr = f_readdir(dp, &fno);
		ERROR_CHECK(fr);

		if (fno.fname[0] == '\0') {
			break;
		}

		int len = strlen(fno.fname);

		if (len > len_name - 1) {
			i--;
			continue;
		}

		if (strcmp(fno.fname + len - 4, ".wav") != 0) {
			i--;
			continue;
		}

		strcpy(names + i*len_name, fno.fname);
	}

	*n_names = i;
}

static void load_file(const char* filename)
{
	FRESULT fr;

	player_stop();
	f_close(&fil);

	LOG_INFO("Load: %s", filename);

	fr = f_open(&fil, filename, FA_READ);

	LOG_DEBUG("Open: %d", fr);

	ERROR_CHECK(fr);

	ERROR_CHECK(player_load(&fil));

	const player_info_t* info = player_get_info();
	LOG_INFO("Play load: %d", info->duration);
}

static void load_next(int sign)
{
	if (sign > 0) {
		p_file++;
		if (p_file >= n_files) {
			p_file = 0;
		}
	} else if (sign < 0) {
		if (p_file == 0) {
			p_file = n_files;
		}
		p_file--;
	}

	load_file(files + p_file*MAX_LEN);
}

/** Main function *************************************************************/
int main(void)
{
	clock_initialization();

#ifdef LOG_ENABLED
	uart_init(UART1_R, 115200);
#endif

	INTERRUPTS_SET_MVEC();

	LOG_INFO(VERSION_INFO);

	timeout_init();
	pwm_init();
	controls_init(controls_cb);

	player_conf_t conf;
	conf.cb = player_cb;
	conf.left_pwm = OC1_R;
	conf.right_pwm = OC2_R;
	player_init(&conf);

	mmc_init();
	i2c_init(I2C1_R, I2C_FREQ_400K);

	FRESULT fr;
	FATFS fs;

	fr = f_mount(&fs, "", 0);

	LOG_DEBUG("Mount: %d", fr);

	display_conf_t dconf;
	dconf.i2c = I2C1_R;
	dconf.addr = DISPLAY_ADDR_1;
	display_init(&display, &dconf);

	ui_init(&ui, &display);

	DIR dp;

	fr = f_opendir(&dp, "/");
	LOG_DEBUG("Opendir: %d", fr);

	n_files = MAX_FILES;
	ls_wav(&dp, files, MAX_LEN, &n_files);

	LOG_DEBUG("Ls: %d", n_files);

	for (int i = 0; i < n_files; i++) {
		LOG_DEBUG("File %d: %s", i, files + i*MAX_LEN);
	}

	if (n_files == 0) {
		ERROR_CHECK(ERR_INVALD_DATA);
	}

	load_file(files);

	for(;;) {
		player_fire();
		show_progress();
		controls_fire();
		ui_play(&ui);
	}

	return 0;
}
