#include "log.h"

/* Non-Maskable Interrupt Handler */
void _nmi_handler()
{
	LOG_ERROR("NMI HANDLER");
	for(;;);
}

void _on_reset()
{

}

void _on_bootstrap()
{

}

void user_isr(void)
{
	return;
}
