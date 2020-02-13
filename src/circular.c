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

	if (wpos >= self->len) {
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
	if (len == 0) {
		return SUCCESS;
	}

	if (len > circular_used(self)) {
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

	if (rpos >= self->len) {
		rpos = 0;
	}

	self->read = rpos;
	return SUCCESS;
}

void circular_skip(circular_t* self, uint32_t len)
{
	if (len == 0) {
		return;
	}

	uint32_t rpos = self->read;
	uint32_t wpos = self->write;

	if(rpos < wpos) {
		rpos += len;
	} else {
		uint32_t end = self->len - rpos;
		if(end >= len) {
			wpos += len;
		} else {
			rpos = len - end;
		}
	}

	if (rpos >= self->len) {
		rpos = 0;
	}

	self->read = rpos;
}

void circular_add(circular_t* self, uint8_t data)
{
	self->data[self->write++] = data;

	if (self->write >= self->len) {
		self->write = 0;
	}
}
