#include "ui.h"
#include "display.h"
#include "display_data.h"
#include "timeout.h"
#include <string.h>

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/
err_t ui_init(ui_t* self, display_t* display)
{
	if (self == NULL || display == NULL) {
		return ERR_NULL;
	}

	self->display = display;

	display_drawtext(self->display, "Hello!.wav", 0, 3);
	display_drawtext(self->display, "2:51", 4, 12);
	display_drawtext(self->display, "play  stop", 7, 3);
	display_drawicon(self->display, 7, 0, sym_left);
	display_drawicon(self->display, 7, 8, sym_ltail);
	display_drawicon(self->display, 7, 14*8, sym_right);
	display_drawicon(self->display, 7, 13*8, sym_rtail);

	display_drawicon(self->display, 2, 7*8, sym_play);
	display_drawicon(self->display, 2, 9*8, sym_stop);
	display_drawicon(self->display, 2, 11*8, sym_pause);
	display_drawicon(self->display, 2, 13*8, sym_resume);

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


	return SUCCESS;
}

err_t ui_loadsong(ui_t* self, const char* name, const player_info_t* info)
{

	return ERR_NOT_IMPLEMENTED;
}

void ui_setprogress(ui_t* self, uint8_t progress)
{

}

void ui_play(ui_t* self)
{
	static uint8_t progress = 0;

	static timeout_t time;

	if(timeout_check(&time)) {
		return;
	}

	timeout_start(&time, 100);

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

	progress++;
	if (progress > 60) {
		progress = 0;
	}

}

void ui_pause(ui_t* self)
{

}
