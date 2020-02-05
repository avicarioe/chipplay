#include "log.h"
#include "util.h"
#include <stdint.h>

void error_fault_handler(err_t err, uint32_t line, const char* file)
{
	// TODO Disable irq
	LOG_ERROR("Fatal error %d at %s:%u", err, file, (unsigned int)line);

#ifndef DEBUG
	LOG_WARN("System reset");
	// TODO System reset
	for(;;);
#else
	for(;;);
#endif // DEBUG
}
