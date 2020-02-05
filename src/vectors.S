# vectors.S
# This file written 2020 by Alejandro Vicario

.macro HANDLER num, name
.align 4
.section .vector_new_\num, "ax", @progbits
.global __vector_\num
__vector_\num:
j \name\()_IRQHandler
.text
.weak \name\()_IRQHandler
eret
nop
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
