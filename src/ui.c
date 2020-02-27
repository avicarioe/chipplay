#include "ui.h"
#include "display.h"
#include "display_data.h"
#include "timeout.h"
#include <string.h>
#include <stdio.h>

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static void set_time(ui_t* self, uint16_t time);
static void progress_rect(ui_t* self);
static void text_center(ui_t* self, const char* text, uint8_t line);

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
static void set_time(ui_t* self, uint16_t time)
{
	char buffer[6];

	uint8_t min = time/60;
	uint8_t seg = time % 60;

	min = min % 100;

	sprintf(buffer, "%2d:%02d", min, seg);

	display_drawtext(self->display, buffer, 4, 11);
}

static void progress_rect(ui_t* self)
{
	uint8_t buffer[64];
	display_rect_t rect;
	rect.line = 3;
	rect.width = sizeof(buffer);
	rect.x = 20;
	memset(buffer, 0x40, sizeof(buffer));
	buffer[0] = 0xC0;
	buffer[sizeof(buffer) - 1] = 0xC0;

	display_drawrect(self->display, &rect, buffer);

	rect.line = 4;
	buffer[0] = 0x7F;
	buffer[1] = 0x40;
	buffer[sizeof(buffer) - 1] = 0x7F;

	memset(buffer + 2, 0x40, 60);

	display_drawrect(self->display, &rect, buffer);

}

static void text_center(ui_t* self, const char* text, uint8_t line)
{
	uint8_t len = strlen(text);

	if (len > DISPLAY_COLUMNS) {
		len = DISPLAY_COLUMNS;
	}

	char buffer[DISPLAY_COLUMNS];
	memcpy(buffer, text, len);
	buffer[len] = '\0';

	uint8_t offset = DISPLAY_COLUMNS - len;
	offset /= 2;
	
	display_clear_line(self->display, line);
	display_drawtext(self->display, buffer, line, offset);
}

void ui_notify(ui_t* self, const char* text)
{
	text_center(self, text, 2);

	timeout_start(&self->notify_timer, UI_NOTIFY_TIME);
	self->notify = true;
}

/** Public functions **********************************************************/
err_t ui_init(ui_t* self, display_t* display)
{
	if (self == NULL || display == NULL) {
		return ERR_NULL;
	}

	self->display = display;

	self->name_len = 0;
	self->duration = 0;
	self->notify = false;
	self->scroll_pos = 0;
	self->scroll_enable = false;


	return SUCCESS;
}

err_t ui_loadsong(ui_t* self, const char* name, const player_info_t* info)
{
	uint8_t len = strlen(name);
	len -= 4;

	if (len > UI_NAME_MAX) {
		len = UI_NAME_MAX;
	}

	memcpy(self->name, name, len);
	self->name[len] = '\0';

	self->name_len = len;

	if (self->name_len > DISPLAY_COLUMNS) {
		display_clear_line(self->display, 0);
		self->scroll_pos = 40;
		self->scroll_enable = true;
	} else {
		self->scroll_enable = false;
		text_center(self, self->name, 0);
	}

	progress_rect(self);

	self->duration = info->duration;

	set_time(self, self->duration);

	display_drawicon(self->display, 4, 5, sym_stop);

	display_clear_line(self->display, 7);
	display_drawicon(self->display, 7, 40, sym_play);
	display_drawicon(self->display, 7, 80, sym_stop);
	display_drawicon(self->display, 7, 0, sym_left);
	display_drawicon(self->display, 7, 8, sym_ltail);
	display_drawicon(self->display, 7, 15*8, sym_right);
	display_drawicon(self->display, 7, 14*8, sym_rtail);

	return SUCCESS;
}

void ui_setprogress(ui_t* self, uint16_t elapsed)
{
	uint8_t progress = (elapsed*60)/self->duration;

	uint8_t buffer[64];
	display_rect_t rect;
	rect.line = 4;
	rect.width = sizeof(buffer);
	rect.x = 20;
	buffer[0] = 0x7F;
	buffer[1] = 0x40;
	buffer[sizeof(buffer) - 1] = 0x7F;

	memset(buffer + 2, 0x5F, progress);
	memset(buffer + progress + 2, 0x40, sizeof(buffer) - progress - 3);

	display_drawrect(self->display, &rect, buffer);

	set_time(self, elapsed);
}

void ui_play(ui_t* self)
{
	display_drawicon(self->display, 4, 5, sym_play);

	display_clear_line(self->display, 7);
	display_drawicon(self->display, 7, 40, sym_pause);
	display_drawicon(self->display, 7, 80, sym_stop);
	display_drawicon(self->display, 7, 8, sym_get('+'));
	display_drawicon(self->display, 7, 14*8, sym_get('-'));

	ui_notify(self, "Play!");
}

void ui_pause(ui_t* self)
{
	display_drawicon(self->display, 4, 5, sym_pause);

	display_clear_line(self->display, 7);
	display_drawicon(self->display, 7, 40, sym_resume);
	display_drawicon(self->display, 7, 80, sym_stop);
	display_drawicon(self->display, 7, 8, sym_get('+'));
	display_drawicon(self->display, 7, 14*8, sym_get('-'));

	ui_notify(self, "Pause");
}

void ui_volume(ui_t* self, uint8_t volume)
{

	display_clear_line(self->display, 1);

	char buffer[8];

	sprintf(buffer, "Vol: %02d", volume);
	
	ui_notify(self, buffer);
}

void ui_fire(ui_t* self)
{
	if(self->scroll_enable && !timeout_check(&self->fps_timer)) {
		timeout_start(&self->fps_timer, UI_FPS_TIME);

		uint8_t off = 0;
		int8_t pos_x = self->scroll_pos;
		if (self->scroll_pos < 0) {
			off = -self->scroll_pos/8;
			pos_x = self->scroll_pos%8;
		}

		display_drawtext_x(self->display, self->name + off, 0, pos_x);
		
		self->scroll_pos--;

		if (self->scroll_pos < -self->name_len*8) {
			self->scroll_pos = 127;
		}
	}

	if(self->notify && !timeout_check(&self->notify_timer)) {
		self->notify = false;
		display_clear_line(self->display, 2);
	}
}
