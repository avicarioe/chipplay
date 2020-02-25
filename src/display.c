#include "display.h"
#include "timeout.h"

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

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/

static void gpio_init(display_t* self);
static void display_setup(display_t* self);
static void send_command(display_t* self, uint8_t cmd);

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

	display_clear(self);

	self->vbat_port->PORTCLR = 1 << self->vbat_pin;
	timeout_delay(100);

	send_command(self, CMD_SEGREMAP | 1);
	send_command(self, CMD_COMSCANDEC);

	send_command(self, CMD_SETCOMPINS);
	send_command(self, 0x20);

	send_command(self, CMD_DISPLAYON);

	send_command(self, CMD_DISPLAYALLON);
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
void display_drawrect(display_t* self, display_rect_t* rect);
void display_drawtext(display_t* self, const char* text, uint8_t line, uint8_t pos);
void display_drawicon(display_t* self, const display_rect_t* rect, const uint8_t* data);

void display_clear(display_t* self)
{

}
