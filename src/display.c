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

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static void gpio_init(display_t* self);
static void display_setup(display_t* self);
static void send_command(display_t* self, uint8_t cmd);
static void send_data(display_t* self, const uint8_t* data, uint8_t len);
static void set_cursor(display_t* self, uint8_t line, uint8_t x);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static void display_setup(display_t* self) {

	self->vdd_port->PORTCLR = 1 << self->vdd_pin;
	timeout_delay(100);

	send_command(self, CMD_DISPLAYOFF);
	self->res_port->PORTCLR = 1 << self->res_pin;
	timeout_delay(10);
	self->res_port->PORTSET = 1 << self->res_pin;
	timeout_delay(10);

	send_command(self, CMD_CHARGEPUMP);
	send_command(self, 0x14);

	send_command(self, CMD_SETPRECHARGE);
	send_command(self, 0xF1);

	send_command(self, CMD_SETDISPLAYOFFSET);
	send_command(self, 0);

	send_command(self, CMD_SETSTARTLINE | 0x00);

	send_command(self, CMD_SETCONTRAST);
	send_command(self, 0x7F);

	send_command(self, CMD_MEMORYMODE);
	send_command(self, 0);

	//display_clear(self);

	self->vbat_port->PORTCLR = 1 << self->vbat_pin;
	timeout_delay(100);

	send_command(self, CMD_SEGREMAP | 1);
	send_command(self, CMD_COMSCANDEC);

	send_command(self, CMD_SETCOMPINS);
	send_command(self, 0x20);

	send_command(self, CMD_DISPLAYON);
}

static void gpio_init(display_t* self)
{
	self->vdd_port->TRISCLR = 1 << self->vdd_pin;
	self->vdd_port->PORTSET = 1 << self->vdd_pin;

	self->vbat_port->TRISCLR = 1 << self->vbat_pin;
	self->vbat_port->PORTSET = 1 << self->vbat_pin;

	self->res_port->TRISCLR = 1 << self->res_pin;
	self->res_port->PORTCLR = 1 << self->res_pin;

	self->dc_port->TRISCLR = 1 << self->dc_pin;
	self->dc_port->PORTCLR = 1 << self->dc_pin;
}

static void send_command(display_t* self, uint8_t cmd)
{
	self->dc_port->PORTCLR = 1 << self->dc_pin;
	uint8_t rx;
	spi_single(self->spi, cmd, &rx);
}

static void send_data(display_t* self, const uint8_t* data, uint8_t len)
{
	self->dc_port->PORTSET = 1 << self->dc_pin;
	spi_xfer_t xfer;
	xfer.rx_buff = NULL;
	xfer.rx_len = 0;
	xfer.tx_buff = data;
	xfer.tx_len = len;
	spi_xfer(self->spi, &xfer);
	self->dc_port->PORTCLR = 1 << self->dc_pin;
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
	self->spi = conf->spi;
	self->vdd_port = conf->vdd_port;
	self->vdd_pin = conf->vdd_pin;
	self->vbat_port = conf->vbat_port;
	self->vbat_pin = conf->vbat_pin;
	self->res_port = conf->res_port;
	self->res_pin = conf->res_pin;
	self->dc_port = conf->dc_port;
	self->dc_pin = conf->dc_pin;
	gpio_init(self);
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
