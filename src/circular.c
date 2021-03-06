/*
@file: circular.c
Copyright (C) 2020 by Alejandro Vicario, Xiaoyu Wang and chipPLAY contributors.
This file is part of the chipPLAY project.
ChipPLAY is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ChipPLAY is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with ChipPlay.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "circular.h"
#include "string.h"

/** Global variables **********************************************************/

/** Function prototypes *******************************************************/

/** Callback definitions ******************************************************/

/** Function definitions ******************************************************/

/** Public functions **********************************************************/
void circular_init(circular_t* self, uint8_t* data, uint32_t len)
{
	self->data = data;
	self->len = len;
	self->write = 0;
	self->read = 0;

}

uint32_t circular_used(circular_t* self)
{
	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos <= wpos) {
		return wpos - rpos;
	} else {
		return self->len - (rpos - wpos);
	}
}

uint8_t circular_get(circular_t* self)
{
	if(self->read == self->write) {
		return 0;
	}

	uint8_t sample = self->data[self->read++];

	if(self->read >= self->len) {
		self->read = 0;
	}

	return sample;
}

uint32_t circular_free(circular_t* self)
{
	return self->len - circular_used(self);
}

err_t circular_write(circular_t* self, const uint8_t* data, uint32_t len)
{
	if(circular_free(self) < len) {
		return ERR_NO_MEM;
	}

	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos > wpos) {
		memcpy(self->data + wpos, data, len);
		wpos += len;
	} else {
		uint32_t end = self->len - wpos;
		if(end >= len) {
			memcpy(self->data + wpos, data, len);
			wpos += len;
		} else {
			memcpy(self->data + wpos, data, end);
			memcpy(self->data, data + end, len - end);
			wpos = len - end;
		}
	}

	if(wpos >= self->len) {
		wpos = 0;
	}
	self->write = wpos;
	return SUCCESS;
}

void circular_clear(circular_t* self)
{
	self->read = 0;
	self->write = 0;
}

err_t circular_read(circular_t* self, uint8_t* data, uint32_t len)
{
	if(len == 0) {
		return SUCCESS;
	}

	if(len > circular_used(self)) {
		return ERR_NO_MEM;
	}

	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos < wpos) {
		memcpy(data, self->data + rpos, len);
		rpos += len;
	} else {
		uint32_t end = self->len - rpos;
		if(end >= len) {
			memcpy(data, self->data + rpos, len);
			wpos += len;
		} else {
			memcpy(data, self->data + rpos, end);
			memcpy(data + end, self->data, len - end);
			rpos = len - end;
		}
	}

	if(rpos >= self->len) {
		rpos = 0;
	}

	self->read = rpos;
	return SUCCESS;
}

err_t circular_peek(circular_t* self, uint8_t* data, uint32_t len)
{
	if(len == 0) {
		return SUCCESS;
	}

	if(len > circular_used(self)) {
		return ERR_NO_MEM;
	}

	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos < wpos) {
		memcpy(data, self->data + rpos, len);
	} else {
		uint32_t end = self->len - rpos;
		if(end >= len) {
			memcpy(data, self->data + rpos, len);
		} else {
			memcpy(data, self->data + rpos, end);
			memcpy(data + end, self->data, len - end);
		}
	}

	return SUCCESS;
}

void circular_skip(circular_t* self, uint32_t len)
{
	if(len == 0) {
		return;
	}

	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos < wpos) {
		rpos += len;
	} else {
		uint32_t end = self->len - rpos;
		if(end >= len) {
			rpos += len;
		} else {
			rpos = len - end;
		}
	}

	if(rpos >= self->len) {
		rpos = 0;
	}

	self->read = rpos;
}

void circular_add(circular_t* self, uint8_t data)
{
	self->data[self->write++] = data;

	if(self->write >= self->len) {
		self->write = 0;
	}
}

void circular_skip_one(circular_t* self)
{
	self->read++;
	if(self->read >= self->len) {
		self->read = 0;
	}
}
