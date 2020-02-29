/*
@file: interrupts.h
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

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#define __ISR(name) void __attribute__((interrupt)) name ## _IRQHandler(void)


#define DISABLE_IRQ()    asm volatile("di")
#define ENABLE_IRQ()     asm volatile("ei")

#define INTERRUPTS_SET_MVEC()      \
	DISABLE_IRQ();                 \
	INTCONSET = PIC32_INTCON_MVEC; \
	ENABLE_IRQ();

#define IRQ_PRIORITY_CRITICAL      (7)
#define IRQ_PRIORITY_VERY_HIGH     (6)
#define IRQ_PRIORITY_HIGH          (5)
#define IRQ_PRIORITY_MEDIUM        (4)
#define IRQ_PRIORITY_LOW           (3)
#define IRQ_PRIORITY_VERY_LOW      (2)

#endif // __INTERRUPTS_H

