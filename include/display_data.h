#ifndef DISPLAY_DATA_H__
#define DISPLAY_DATA_H__

#include <stdint.h>

#define DISPLAY_FONT_SIZE (96)
#define DISPLAY_FONT_MIN  (' ')

extern const uint8_t display_font[DISPLAY_FONT_SIZE*8];

extern const uint8_t sym_left[8];
extern const uint8_t sym_right[8];
extern const uint8_t sym_ltail[8];
extern const uint8_t sym_rtail[8];
extern const uint8_t sym_play[8];
extern const uint8_t sym_resume[8];
extern const uint8_t sym_pause[8];
extern const uint8_t sym_stop[8];

#endif // DISPLAY_DATA_H__
