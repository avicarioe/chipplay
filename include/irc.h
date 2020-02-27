#ifndef IRC_H__
#define IRC_H__

#include <stdbool.h>
#include <stdint.h>

typedef void (*irc_cb_t)(uint16_t evt);

void irc_init(irc_cb_t cb);

void irc_fire();

#endif // IRC_H__
