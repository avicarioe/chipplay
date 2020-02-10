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

