#include <stdio.h>
#include <uart.h>

/** Function prototypes *******************************************************/
static size_t write_uart1(FILE* instance, const char* bp, size_t n);
static size_t read_uart1(FILE* instance, char* bp, size_t n);

/** Global variables **********************************************************/
static struct File_methods _uart_file_methods = {
	.read = read_uart1,
	.write = write_uart1,
};

static FILE _std = {
	.vmt = &_uart_file_methods,
};

FILE* const stdin = &_std;
FILE* const stdout = &_std;
FILE* const stderr = &_std;

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions *************************************************************/


static size_t write_uart1(FILE* instance, const char* bp, size_t n)
{
	int i;
	for(i = 0; i < n; i++) {

		uart_tx(UART1_R, bp[i]);

	}

	return n;
}

static size_t read_uart1(FILE* instance, char* bp, size_t n)
{
	return 0;
}
