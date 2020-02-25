#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "spi.h"


typedef struct display_t {
	SPI_reg_t* spi;
	GPIO_reg_t* vdd_port;
	uint8_t vdd_pin;
	GPIO_reg_t* vbat_port;
	uint8_t vbat_pin;
	GPIO_reg_t* res_port;
	uint8_t res_pin;
	GPIO_reg_t* dc_port;
	uint8_t dc_pin;
} display_t;

typedef struct display_conf_t {
	SPI_reg_t* spi;
	GPIO_reg_t* vdd_port;
	uint8_t vdd_pin;
	GPIO_reg_t* vbat_port;
	uint8_t vbat_pin;
	GPIO_reg_t* res_port;
	uint8_t res_pin;
	GPIO_reg_t* dc_port;
	uint8_t dc_pin;
} display_conf_t;

typedef struct display_rect_t {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} display_rect_t;

err_t display_init(display_t* self, const display_conf_t* conf);
void display_drawrect(display_t* self, display_rect_t* rect);
void display_drawtext(display_t* self, const char* text, uint8_t line, uint8_t pos);
void display_drawicon(display_t* self, const display_rect_t* rect, const uint8_t* data);
void display_clear(display_t* self);

#endif // DISPLAY_H__