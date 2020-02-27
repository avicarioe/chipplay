#define LOG_MODULE_NAME "i2c"
#include "log.h"

#include "display.h"
#include "display_data.h"
#include "timeout.h"
#include <string.h>

#define CMD_SETLOWCOLUMN         0x00
#define CMD_SETHIGHCOLUMN        0x10
#define CMD_MEMORYMODE           0x20
#define CMD_COLUMNADDR           0x21
#define CMD_PAGEADDR             0x22
#define CMD_SETSTARTLINE         0x40
#define CMD_DEFAULT_ADDRESS      0x78
#define CMD_SETCONTRAST          0x81
#define CMD_CHARGEPUMP           0x8D
#define CMD_SEGREMAP             0xA0
#define CMD_DISPLAYALLON_RESUME  0xA4
#define CMD_DISPLAYALLON         0xA5
#define CMD_NORMALDISPLAY        0xA6
#define CMD_INVERTDISPLAY        0xA7
#define CMD_SETMULTIPLEX         0xA8
#define CMD_DISPLAYOFF           0xAE
#define CMD_DISPLAYON            0xAF
#define CMD_SETPAGE              0xB0
#define CMD_COMSCANINC           0xC0
#define CMD_COMSCANDEC           0xC8
#define CMD_SETDISPLAYOFFSET     0xD3
#define CMD_SETDISPLAYCLOCKDIV   0xD5
#define CMD_SETPRECHARGE         0xD9
#define CMD_SETCOMPINS           0xDA
#define CMD_SETVCOMDETECT        0xDB
#define CMD_SWITCHCAPVCC         0x02
#define CMD_NOP                  0xE3

#define START_CMD     0x00
#define START_DATA    0x40

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static void display_setup(display_t* self);
static void send_command(display_t* self, uint8_t cmd);
static void send_data(display_t* self, const uint8_t* data, uint8_t len);
static void set_cursor(display_t* self, uint8_t line, uint8_t x);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static void display_setup(display_t* self) {

	timeout_delay(100);

	send_command(self, CMD_DISPLAYOFF);
	timeout_delay(10);

	send_command(self, CMD_NORMALDISPLAY);
	send_command(self, CMD_SETMULTIPLEX);
	send_command(self, 63);
	send_command(self, CMD_SETDISPLAYCLOCKDIV);
	send_command(self, 0x80);
	send_command(self, CMD_SETVCOMDETECT);
	send_command(self, 0x20);
	send_command(self, CMD_CHARGEPUMP);
	send_command(self, 0x14);
	send_command(self, CMD_SETPRECHARGE);
	send_command(self, 0x22);
	send_command(self, CMD_SETDISPLAYOFFSET);
	send_command(self, 0);
	//send_command(self, CMD_SETSTARTLINE | 0x00);
	send_command(self, CMD_SETCONTRAST);
	send_command(self, 0x7F);
	send_command(self, CMD_MEMORYMODE);
	send_command(self, 0);

	display_clear(self);
	timeout_delay(10);

	send_command(self, CMD_SEGREMAP | 0x00);
	send_command(self, CMD_COMSCANINC);
	send_command(self, CMD_SETCOMPINS);
	send_command(self, 0x12);

	send_command(self, CMD_DISPLAYON);
}

static void send_command(display_t* self, uint8_t cmd)
{
	err_t rc = i2c_tx(self->i2c, self->addr, START_CMD, &cmd, 1, false);
	if (rc != SUCCESS) {
		LOG_WARN("Send command: %d", rc);
	}
}

static void send_data(display_t* self, const uint8_t* data, uint8_t len)
{
	err_t rc = i2c_tx(self->i2c, self->addr, START_DATA, data, len, true);
	if (rc != SUCCESS) {
		LOG_WARN("Send data: %d", rc);
	}
}

static void set_cursor(display_t* self, uint8_t line, uint8_t x)
{
	send_command(self, CMD_SETPAGE | line);

	send_command(self, CMD_SETLOWCOLUMN | (x & 0x0F));
	send_command(self, CMD_SETHIGHCOLUMN | (x >> 4));
}

/** Public functions **********************************************************/
err_t display_init(display_t* self, const display_conf_t* conf)
{
	if (self == NULL || conf == NULL) {
		return ERR_NULL;
	}

	self->i2c = conf->i2c;
	self->addr = conf->addr;

	display_setup(self);

	return SUCCESS;
}

void display_drawrect(display_t* self, const display_rect_t* rect,
		const uint8_t* data)
{
	ASSERT(rect->line < DISPLAY_LINES);
	ASSERT(rect->width + rect->x <= DISPLAY_WIDTH);

	set_cursor(self, rect->line, rect->x);

	send_data(self, data, rect->width);
}

void display_drawtext(display_t* self, const char* text, uint8_t line,
		uint8_t pos)
{
	ASSERT(line < DISPLAY_LINES);
	ASSERT(pos < DISPLAY_COLUMNS);

	int len = strlen(text);

	if (len > 16 - pos) {
		len = 16 - pos;
	}

	uint8_t buffer[len*8];

	for (int i = 0; i < len; i++) {
		char c = text[i];

		if (c < DISPLAY_FONT_MIN) {
			c = '?';
		}

		c = c - DISPLAY_FONT_MIN;

		for (int j = 0; j < 8; j++) {
			buffer[i*8 + j] = display_font[c*8 + j];
		}
	}

	set_cursor(self, line, pos*8);
	send_data(self, buffer, sizeof(buffer));
}

void display_drawicon(display_t* self, uint8_t line, uint8_t x,
		const uint8_t* data)
{
	display_rect_t rect;
	rect.line = line;
	rect.x = x;
	rect.width = 8;

	display_drawrect(self, &rect, data);
}

void display_clear(display_t* self)
{
	uint8_t buffer[DISPLAY_WIDTH];
	memset(buffer, 0, sizeof(buffer));
	
	for (int i = 0; i < DISPLAY_LINES; i++) {
		set_cursor(self, i, 0);
		send_data(self, buffer, sizeof(buffer));
	}
}

void display_clear_line(display_t* self, uint8_t line)
{
	uint8_t buffer[DISPLAY_WIDTH];
	memset(buffer, 0, sizeof(buffer));

	set_cursor(self, line, 0);
	send_data(self, buffer, sizeof(buffer));
}
