#ifndef UI_H__
#define UI_H__

#include <stdbool.h>
#include <stdint.h>
#include "util.h"
#include "display.h"
#include "player.h"

typedef struct ui_t {
	display_t* display;
} ui_t;

err_t ui_init(ui_t* self, display_t* display);
err_t ui_loadsong(ui_t* self, const* char name, const player_info_t* info);
void ui_setprogress(ui_t* self, uint8_t progress);
void ui_play(ui_t* self);
void ui_pause(ui_t* self);

#endif // UI_H__
