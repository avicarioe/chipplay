#ifndef __MMC_H
#define __MMC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MMC_STATUS_NOINIT,
	MMC_STATUS_INIT,
	MMC_STATUS_NODISK,
} mmc_status_t;

typedef enum {
	MMC_CT_MMC,
	MMC_CT_SD1,
	MMC_CT_SD2,
	MMC_CT_BLOCK,
} mmc_ct_t;

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC      0x01              /* MMC ver 3 */
#define CT_SD1      0x02              /* SD ver 1 */
#define CT_SD2      0x04              /* SD ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2)   /* SD */
#define CT_BLOCK    0x08              /* Block addressing */

void mmc_init();

#ifdef __cplusplus
}
#endif

#endif
