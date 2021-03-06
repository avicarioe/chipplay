/*
@file: vectors.S
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

# vectors.S
# This file written 2020 by Alejandro Vicario

.macro HANDLER num, name
.align 4
.section .vector_new_\num, "ax", @progbits
.global __vector_\num
__vector_\num:
j \name\()_IRQHandler
.weak \name\()_IRQHandler
.set \name\()_IRQHandler, DefaultHandler
.endm

.align 4
.global __use_isr_install
__use_isr_install:
HANDLER 0 CORETIMER
HANDLER 1 CSWI0
HANDLER 2 CSWI1
HANDLER 3 EXT0
HANDLER 4 TIMER1
HANDLER 5 CAPTURE1
HANDLER 6 COMPARE1
HANDLER 7 EXT1

HANDLER 8 TIMER2
HANDLER 9 CAPTURE2
HANDLER 10 COMPARE2
HANDLER 11 EXT2
HANDLER 12 TIMER3
HANDLER 13 CAPTURE3
HANDLER 14 COMPARE3
HANDLER 15 EXT3

HANDLER 16 TIMER4
HANDLER 17 CAPTURE4
HANDLER 18 COMPARE4
HANDLER 19 EXT4
HANDLER 20 TIMER5
HANDLER 21 CAPTURE5
HANDLER 22 COMPARE5
HANDLER 23 SPI1

HANDLER 24 MULTI1
HANDLER 25 I2C1
HANDLER 26 INPUTCHANGE
HANDLER 27 ADC1
HANDLER 28 PARALLEL
HANDLER 29 COMPARATOR1
HANDLER 30 COMPARATOR2
HANDLER 31 MULTI2

HANDLER 32 MULTI13
HANDLER 33 I2C2
HANDLER 34 FAILSAFE
HANDLER 35 RTCC
HANDLER 36 DMA0
HANDLER 37 DMA1
HANDLER 38 DMA2
HANDLER 39 DMA3

HANDLER 40 DMA4
HANDLER 41 DMA5
HANDLER 42 DMA6
HANDLER 43 DMA7
HANDLER 44 FLASH
HANDLER 45 USB
HANDLER 46 CAN1
HANDLER 47 CAN2

HANDLER 48 ETHERNET
HANDLER 49 UART4
HANDLER 50 UART6
HANDLER 51 UART7

.text
.align 4
# Exceptions are handled here (trap, syscall, etc)
.section .gen_handler, "ax", @progbits
.set noreorder
.ent _gen_exception
_gen_exception:
mfc0 $k0, $14, 0
addi $k0, $k0, 4
mtc0 $k0, $14, 0
eret
nop

.end _gen_exception

.section _def_handler, "ax", @progbits
DefaultHandler:
j DefaultHandler
nop
