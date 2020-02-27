#ifndef UI_H__
#define UI_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "display.h"
#include "player.h"
#include "timeout.h"

#define UI_NAME_MAX     (DISPLAY_COLUMNS)
#define UI_NOTIFY_TIME  (2500)
#define UI_FPS_TIME     (200)

typedef struct ui_t {
	display_t* display;
	char name[UI_NAME_MAX + 1];
	uint8_t name_len;
	uint16_t duration;
	timeout_t notify_timer;
	timeout_t fps_timer;
	bool notify;
} ui_t;

err_t ui_init(ui_t* self, display_t* display);
err_t ui_loadsong(ui_t* self, const char* name, const player_info_t* info);
void ui_setprogress(ui_t* self, uint16_t elapsed);
void ui_play(ui_t* self);
void ui_pause(ui_t* self);
void ui_volume(ui_t* self, uint8_t volume);
void ui_fire(ui_t* self);
void ui_notify(ui_t* self, const char* text);

#endif // UI_H__
