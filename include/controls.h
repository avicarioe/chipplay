#ifndef CONTROLS_H__
#define CONTROLS_H__

#include <stdbool.h>
#include <stdint.h>

#define CONTROLS_N_EVT (4)

typedef void (*controls_cb_t)(uint8_t evt);

void controls_init(controls_cb_t cb);

void controls_fire();

#endif // CONTROLS_H__
