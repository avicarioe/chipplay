/*
@file: mmc.h
Copyright (C) 2020 by Alejandro Vicario, Xiaoyu Wang and chipPLAY contributors.
This file is part of the chipPLAY project.
ChipPLAY is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ChipPLAY is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with ChipPlay.  If not, see <http://www.gnu.org/licenses/>.
*/

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
