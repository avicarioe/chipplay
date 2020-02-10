#include "ff.h"
#include "diskio.h"
#include "timeout.h"
#include "spi.h"
#include "string.h"
#include "mmc.h"

#define PIN_CS_N     (2)
#define PIN_CS_REG   (GPIOE_R)

#define SPI_REG      SPI2_R

/* Socket controls  (Platform dependent) */
#define CS_LOW()   PIN_CS_REG->PORTCLR = (1 << PIN_CS_N)
#define CS_HIGH()  PIN_CS_REG->PORTSET = (1 << PIN_CS_N)
#define MMC_CD     (true)
#define MMC_WP     (true)

/* SPI bit rate controls */
#define	FCLK_SLOW()   spi_set_freq(SPI_REG, 250000U);
#define	FCLK_FAST()   spi_set_freq(SPI_REG, 4000000U);

/* Definitions for MMC/SDC command */
#define CMD0   (0)          /* GO_IDLE_STATE */
#define CMD1   (1)          /* SEND_OP_COND */
#define ACMD41 (41|0x80)    /* SEND_OP_COND (SDC) */
#define CMD8   (8)          /* SEND_IF_COND */
#define CMD9   (9)          /* SEND_CSD */
#define CMD10  (10)         /* SEND_CID */
#define CMD12  (12)         /* STOP_TRANSMISSION */
#define ACMD13 (13|0x80)    /* SD_STATUS (SDC) */
#define CMD16  (16)         /* SET_BLOCKLEN */
#define CMD17  (17)         /* READ_SINGLE_BLOCK */
#define CMD18  (18)         /* READ_MULTIPLE_BLOCK */
#define CMD23  (23)         /* SET_BLOCK_COUNT */
#define ACMD23 (23|0x80)    /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)         /* WRITE_BLOCK */
#define CMD25  (25)         /* WRITE_MULTIPLE_BLOCK */
#define CMD41  (41)         /* SEND_OP_COND (ACMD) */
#define CMD55  (55)         /* APP_CMD */
#define CMD58  (58)         /* READ_OCR */

/** Global variables **********************************************************/

static volatile DSTATUS stat = STA_NOINIT; /* Disk status */

static timeout_t timer;

static UINT card_type;

/** Function prototypes *******************************************************/
static void power_on (void);
static void power_off (void);
static BYTE xchg_spi (BYTE dat);
static void rcvr_spi_multi (BYTE* buff, UINT cnt);
static int wait_ready (void);
static void mmc_deselect (void);
static int mmc_select (void);
static int rcvr_datablock (BYTE *buff, UINT btr);
static BYTE send_cmd (BYTE cmd, DWORD arg);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

static void power_on (void)
{
	// Turn socket power on, delay >1ms
	timeout_delay(100);
}

static void power_off (void)
{
	// Turn socket power off
	timeout_delay(100);
}

static BYTE xchg_spi (BYTE dat)
{
	uint8_t rx_buf = 0;
	spi_xfer_t xfer;
	xfer.rx_buff = &rx_buf;
	xfer.tx_buff = &dat;
	xfer.rx_len = 1;
	xfer.tx_len = 1;

	spi_xfer(SPI_REG, &xfer);

	return rx_buf;
}

static void rcvr_spi_multi (BYTE* buff, UINT cnt)
{
	spi_xfer_t xfer;
	xfer.rx_buff = buff;
	xfer.tx_buff = NULL;
	xfer.rx_len = cnt;
	xfer.tx_len = 0;
	spi_xfer(SPI_REG, &xfer);
}

static int wait_ready (void)
{
	// Wait for init with 500ms timeout
	
	BYTE d;

	timeout_start(&timer, 500);

	do {
		d = xchg_spi(0xFF);
	} while ((d != 0xFF) && timeout_check(&timer));

	return (d == 0xFF) ? 1 : 0;
}

static void mmc_deselect (void)
{
	CS_HIGH();
	xchg_spi(0xFF); // Dummy clock
}

static int mmc_select (void)
{
	CS_LOW();
	xchg_spi(0xFF); // Dummy clock

	if (wait_ready()) {
		return 1;
	}

	// Timeout
	mmc_deselect();
	return 0;
}

static int rcvr_datablock (BYTE *buff, UINT btr)
{
	/* Wait for data packet in timeout of 100ms */
	BYTE token;
	
	timeout_start(&timer, 100);
	do {
		token = xchg_spi(0xFF);
	} while ((token == 0xFF) && timeout_check(&timer));

	/* If not valid data token, retutn with error */
	if(token != 0xFE) return 0;

	rcvr_spi_multi(buff, btr);
	xchg_spi(0xFF); // Discard CRC
	xchg_spi(0xFF);

	return 1;
}

static BYTE send_cmd (BYTE cmd, DWORD arg)
{
	BYTE n, res;

	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
	if (cmd & 0x80) {
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) {
			return res;
		}
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		mmc_deselect();
		if (!mmc_select()) {
			return 0xFF;
		}
	}

	/* Send command packet */
	xchg_spi(0x40 | cmd);           // Start + Command index
	xchg_spi((BYTE)(arg >> 24));    // Argument[31..24]
	xchg_spi((BYTE)(arg >> 16));    // Argument[23..16]
	xchg_spi((BYTE)(arg >> 8));     // Argument[15..8]
	xchg_spi((BYTE)arg);            // Argument[7..0]
	n = 0x01;                       // Dummy CRC + Stop
	if (cmd == CMD0) {              // Valid CRC for CMD0(0) + Stop
		n = 0x95;
	}
	if (cmd == CMD8) {              // Valid CRC for CMD8(0x1AA) + Stop
		n = 0x87;
	}
	xchg_spi(n);

	/* Receive command response */
	if (cmd == CMD12) {
		xchg_spi(0xFF);             // Skip a stuff byte on stop to read
	}
	n = 10;                         // Wait for a valid response
	do {
		res = xchg_spi(0xFF);
	} while ((res & 0x80) && --n);

	return res;
}


/** Public functions **********************************************************/

DSTATUS disk_status (BYTE pdrv)
{
	if (pdrv != 0) {       // Supports only single drive
		return STA_NOINIT;
	}

	return stat;
}

DSTATUS disk_initialize (BYTE pdrv)
{
	if (pdrv != 0) {       // Supports only single drive
		return STA_NOINIT;
	}

	if (stat & STA_NODISK) {
		return stat;
	}

	BYTE n, cmd, ty, ocr[4];

	power_on();
	FCLK_SLOW();
	CS_HIGH();
	for (n = 10; n; n--) {   // 80 dummy clocks
		xchg_spi(0xFF);
	}

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {             // Enter Idle state
		timeout_start(&timer, 1000);
		if (send_cmd(CMD8, 0x1AA) == 1) {     /* SDv2? */
			// Get trailing return value of R7 resp
			for (n = 0; n < 4; n++) {
				ocr[n] = xchg_spi(0xFF);
			}

			/* The card can work at vdd range of 2.7-3.6V */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {

				/* Wait for leaving idle state (ACMD41 with HCS bit) */
				while (timeout_check(&timer) && send_cmd(ACMD41, 0x40000000));

				/* Check CCS bit in the OCR */
				if (timeout_check(&timer) && send_cmd(CMD58, 0) == 0) {
					for (n = 0; n < 4; n++) {
						ocr[n] = xchg_spi(0xFF);
					}
					ty = (ocr[0] & 0x40) ? CT_SD2|CT_BLOCK : CT_SD2; // SDv2+
				}
			}
		} else {                              /* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) {   /* SDv1 */
				ty = CT_SD1;
				cmd = ACMD41;
			} else {                          /* MMCv3 */
				ty = CT_MMC;
				cmd = CMD1;
			}

			/* Wait for leaving idle state */
			while (timeout_check(&timer) && send_cmd(cmd, 0));
			/* Set read/write block length to 512 */
			if (!timeout_check(&timer) || send_cmd(CMD16, 512) != 0) ty = 0;
		}
	}

	card_type = ty;
	mmc_deselect();

	if (ty) {
		stat &= ~STA_NOINIT;
		FCLK_FAST();
	} else {
		power_off();
	}

	return stat;
}

DRESULT disk_read (BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
	DWORD sect = (DWORD)sector;

	if (pdrv || !count) {
		return RES_PARERR;
	}
	if (stat & STA_NOINIT) {
		return RES_NOTRDY;
	}

	if (!(card_type & CT_BLOCK)) { // Convert to byte address if needed
		sect *= 512;
	}

	if (count == 1) {              /* Single block read */
		if ((send_cmd(CMD17, sect) == 0) &&
				rcvr_datablock(buff, 512)) {
			count = 0;
		}
	} else {                       /* Multiple block read */
		if (send_cmd(CMD18, sect) == 0) {
			do {
				if (!rcvr_datablock(buff, 512)) {
					break;
				}
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0); // STOP_TRANSMISSION
		}
	}

	mmc_deselect();

	return count ? RES_ERROR : RES_OK;
}

void mmc_init()
{
	PIN_CS_REG->TRISCLR = (1 << PIN_CS_N);
	CS_HIGH();

	spi_init(SPI_REG);

}
