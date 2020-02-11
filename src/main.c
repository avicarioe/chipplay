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

	INTERRUPTS_SET_MVEC();

	LOG_INFO(VERSION_INFO);

	LOG_DEBUG("Debug %s:%d", __FILE__, __LINE__);
	LOG_INFO("Info %s:%d", __FILE__, __LINE__);
	LOG_WARN("Warning %s:%d", __FILE__, __LINE__);
	LOG_ERROR("Error %s:%d", __FILE__, __LINE__);

	timeout_init();
	pwm_init();

	timer16_init(TIMER4_R, PIC32_TPSB_4, 399);
	timer16_start(TIMER4_R);

	DISABLE_IRQ();

	IPCCLR(4) = PIC32_IPC_TxIP_MASK | PIC32_IPC_TxIS_MASK;
	IPCSET(4) = IRQ_PRIORITY_LOW << PIC32_IPC_TxIP_POS;
	IECSET(0) = PIC32_IEC_T4IE;

	ENABLE_IRQ();

	mmc_init();

	FRESULT fr;
	FATFS fs;

	fr = f_mount(&fs, "", 0);

	LOG_INFO("Mount: %d", fr);

	DIR dr;

	fr = f_opendir(&dr, "/");

	LOG_INFO("Opendir: %d", fr);

	FILINFO fno;

	fr = f_readdir(&dr, &fno);

	LOG_INFO("Readdir: %d", fr);

	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);

	FIL fil;
	fr = f_open(&fil, fno.fname, FA_READ);

	LOG_INFO("Open: %d", fr);

	uint8_t buff[fno.fsize + 1];
	UINT br;

	fr = f_read(&fil, buff, fno.fsize, &br);

	LOG_INFO("Read: %d, n: %d", fr, br);

	for (int i=0; i < br; i++){
		LOG_INFO("|%X|     %c", buff[i], (char)buff[i]);
	}

	fr = f_readdir(&dr, &fno);
	LOG_INFO("Readdir: %d", fr);
	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);
	fr = f_readdir(&dr, &fno);
	LOG_INFO("Readdir: %d", fr);
	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);
	fr = f_readdir(&dr, &fno);
	LOG_INFO("Readdir: %d", fr);
	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);
	fr = f_readdir(&dr, &fno);
	LOG_INFO("Readdir: %d", fr);
	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);
	fr = f_readdir(&dr, &fno);
	LOG_INFO("Readdir: %d", fr);
	LOG_INFO("FILE size: %d, name %s", (int)fno.fsize, fno.fname);

	for(;;) {
		LOG_INFO("For");
		timeout_delay(1000);
	}

	return 0;
}

static const uint8_t table[] = {
127, 130, 133, 135, 138, 141, 144, 146, 149, 151, 154, 156, 158, 161, 163,
165, 166, 168, 170, 171, 173, 174, 175, 176, 176, 177, 178, 178, 178, 178,
178, 177, 177, 176, 176, 175, 174, 172, 171, 170, 168, 166, 164, 162, 160,
158, 156, 153, 151, 148, 146, 143, 140, 138, 135, 132, 129, 126, 124, 121,
118, 115, 113, 110, 107, 105, 102, 100, 97, 95, 93, 91, 89, 87, 86, 84, 82,
81, 80, 79, 78, 77, 77, 76, 76, 76, 76, 76, 77, 77, 78, 79, 80, 81, 82, 83,
85, 86, 88, 90, 92, 94, 96, 99, 101, 104, 106, 109, 111, 114, 117, 120, 122,
125
};

__ISR(TIMER4)
{
	static uint32_t t = 0;
	IFSCLR(0) = PIC32_IFS_T4IF;

	uint8_t val = table[t++];
	pwm_set(val);
	if (t > sizeof(table)-1) {
		t = 0;
	}

	return;
}
