#include "log.h"
#include "main.h"
#include "util.h"
#include <stdint.h>

#define ERROR_DISPLAY

void error_fault_handler(err_t err, uint32_t line, const char* file)
{
	// TODO Disable irq
	LOG_ERROR("Fatal error %d at %s:%u", err, file, (unsigned int)line);

#ifndef DEBUG
	LOG_WARN("System reset");
	// TODO System reset
	for(;;);
#else
#ifdef ERROR_DISPLAY

	display_clear(&display);
	display_drawtext(&display, "Error!", 2, 5);
	display_drawtext(&display, "Please, reboot", 4, 1);
	display_drawtext(&display, "the device.", 5, 3);

#endif
	for(;;);
#endif // DEBUG
}
