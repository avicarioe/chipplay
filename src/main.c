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

/** Global variables **********************************************************/
static uint8_t buffer[2048];
static uint8_t pcm1[2048];
static uint8_t pcm2[2048];
static volatile uint16_t samples1;
static volatile uint16_t samples2;
static wave_t wave;

/** Function prototypes *******************************************************/
static void clock_initialization();

/** Callback definitions ******************************************************/
__ISR(TIMER4)
{
	static int reading = 0;
	static uint16_t t = 0;
	IFSCLR(0) = PIC32_IFS_T4IF;


	if (reading == 1) {
		pwm_set(pcm1[t++]);
		if (t >= samples1) {
			samples1 = 0;
			t = 0;
			reading = 2;
		}
	} else if (reading == 2) {
		pwm_set(pcm2[t++]);
		if (t >= samples2) {
			samples2 = 0;
			t = 0;
			reading = 1;
		}
	} else if (reading == 0 && samples1) {
		reading = 1;
	}

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

	timer16_init(TIMER4_R, PIC32_TPSB_4, 452);
	timer16_start(TIMER4_R);

	TIMER16_ENABLE_INT(4, IRQ_PRIORITY_LOW);

	mmc_init();

	FRESULT fr;
	FATFS fs;

	fr = f_mount(&fs, "", 0);

	LOG_INFO("Mount: %d", fr);

	FIL fil;
	fr = f_open(&fil, "senorita.wav", FA_READ);

	LOG_INFO("Open: %d", fr);

	UINT br;

	fr = f_read(&fil, buffer, sizeof(buffer), &br);

	LOG_INFO("Read: %d, n: %d", fr, br);

	uint16_t next;
	err_t ret = wave_init(&wave, buffer, br, &next);

	LOG_INFO("Wave: %d, %d", ret, next);

	LOG_INFO("Info: L: %d, ch: %d, sr: %d, br: %d, al: %d, bps: %d",
			(unsigned int) wave.length, wave.channels,
			(unsigned int) wave.sample_rate,
			(unsigned int) wave.byte_rate,
			wave.align, wave.bpsample);

	uint16_t offset = next;
	wave_io_t io;

	while (true) {

		io.in = buffer + offset;
		io.in_len = sizeof(buffer) - offset;
		io.out = pcm1;
		io.out_len = sizeof(pcm1);
		samples1 = wave_dec(&wave, &io, &next);

		offset += next;

		LOG_INFO("Wave1: %d, %d", samples1, next);

		if (offset == sizeof(buffer)) {
			fr = f_read(&fil, buffer, sizeof(buffer), &br);
			LOG_INFO("Read: %d, n: %d", fr, br);
			offset = 0;
		}

		while (samples2);

		io.in = buffer + offset;
		io.in_len = sizeof(buffer) - offset;
		io.out = pcm2;
		io.out_len = sizeof(pcm2);
		samples2 = wave_dec(&wave, &io, &next);

		LOG_INFO("Wave2: %d, %d", samples2, next);

		offset += next;

		if (offset == sizeof(buffer)) {
			fr = f_read(&fil, buffer, sizeof(buffer), &br);
			LOG_INFO("Read: %d, n: %d", fr, br);
			offset = 0;
		}

		while (samples1);

	}



	for(;;) {
		LOG_INFO("For");
		timeout_delay(1000);
	}

	return 0;
}

