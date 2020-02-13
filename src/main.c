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
#include "timeout.h"
#include "mmc.h"
#include "ff.h"
#include "pwm.h"
#include "wave.h"
#include "circular.h"

/** Global variables **********************************************************/
static uint8_t buffer[1025];
static uint8_t sd_data[2048];
static uint8_t pcm_data[2048];
static wave_t wave;
static circular_t pcm;
static circular_t sd;

/** Function prototypes *******************************************************/
static void clock_initialization();

/** Callback definitions ******************************************************/
__ISR(TIMER4)
{
	IFSCLR(0) = PIC32_IFS_T4IF;

	uint8_t left = circular_get(&pcm);
	uint8_t right = circular_get(&pcm);
	pwm_set((left + right)/2);

	return;
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

/** Main function *************************************************************/
int main(void)
{

	clock_initialization();
	uart_init(UART1_R, 115200);

	INTERRUPTS_SET_MVEC();

	LOG_INFO(VERSION_INFO);

	timeout_init();
	pwm_init();
	circular_init(&pcm, pcm_data, sizeof(pcm_data));
	circular_init(&sd, sd_data, sizeof(sd_data));

	timer16_init(TIMER4_R, PIC32_TPSB_4, 452);

	TIMER16_ENABLE_INT(4, IRQ_PRIORITY_LOW);

	mmc_init();

	FRESULT fr;
	FATFS fs;

	fr = f_mount(&fs, "", 0);

	LOG_DEBUG("Mount: %d", fr);

	FIL fil;
	fr = f_open(&fil, "senfull.wav", FA_READ);

	LOG_DEBUG("Open: %d", fr);

	UINT br;

	fr = f_read(&fil, buffer, sizeof(buffer), &br);
	circular_write(&sd, buffer, br);

	LOG_DEBUG("Read: %d, n: %d", fr, br);

	err_t ret = wave_init(&wave, &sd, &pcm);

	LOG_DEBUG("Wave: %d", ret);

	LOG_INFO("Info: L: %d, ch: %d, sr: %d, br: %d, al: %d, bps: %d",
		(unsigned int) wave.length, wave.channels,
		(unsigned int) wave.sample_rate,
		(unsigned int) wave.byte_rate,
		wave.align, wave.bpsample);

	timer16_set_freq(TIMER4_R, wave.sample_rate);
	timer16_start(TIMER4_R);

	while(!wave_end(&wave)) {

		while(circular_free(&pcm) < sizeof(pcm));

		if (circular_used(&pcm) < 512) {
			uint32_t samples = wave_dec(&wave);
			(void)samples;
		}

		if(circular_free(&sd) > sizeof(buffer)) {
			fr = f_read(&fil, buffer, sizeof(buffer), &br);
			circular_write(&sd, buffer, br);
		}
	}

	while(circular_used(&pcm));

	timer16_stop(TIMER4_R);
	pwm_set(0);

	for(;;) {
		LOG_INFO("For");
		timeout_delay(1000);
	}

	return 0;
}

