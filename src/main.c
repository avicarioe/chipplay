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
static uint8_t buffer[2048];
static uint8_t data[2048];
static uint8_t pcm[512];
static wave_t wave;
static circular_t circular;

/** Function prototypes *******************************************************/
static void clock_initialization();

/** Callback definitions ******************************************************/
__ISR(TIMER4)
{
	IFSCLR(0) = PIC32_IFS_T4IF;
	pwm_set(circular_get(&circular));

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
	circular_init(&circular, data, sizeof(data));

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

	while(!wave_end(&wave)) {

		while(circular_free(&circular) < sizeof(pcm));

		io.in = buffer + offset;
		io.in_len = sizeof(buffer) - offset;
		io.out = pcm;
		io.out_len = sizeof(pcm);
		uint32_t samples = wave_dec(&wave, &io, &next);

		offset += next;

		LOG_INFO("Wave: %d, %d", (unsigned int)samples, next);
		LOG_INFO("Progress: %d", wave_progress(&wave));

		ERROR_CHECK(circular_write(&circular, pcm, samples));

		uint16_t left = sizeof(buffer) - offset;
		if(left < sizeof(buffer)/4) {
			memcpy(buffer, buffer + offset, left);
			fr = f_read(&fil, buffer + left, sizeof(buffer) - left, &br);
			LOG_INFO("Read: %d, n: %d", fr, br);
			offset = 0;
		}
	}

	for(;;) {
		LOG_INFO("For");
		timeout_delay(1000);
	}

	return 0;
}

