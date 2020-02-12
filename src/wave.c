#include "wave.h"

#define FORMAT_PCM 1

#define STR_RIFF   0x46464952
#define STR_WAVE   0x45564157
#define STR_FMT    0x20746D66
#define STR_DATA   0x61746164

typedef union {
	char str[4];
	uint32_t value;
} wave_str_t;

typedef struct wave_chunk_t wave_chunk_t;
typedef struct wave_sub_t wave_sub_t;
typedef struct wave_fmt_t wave_fmt_t;


struct wave_chunk_t {
	wave_str_t riff;
	uint32_t chunk_size;
	wave_str_t wave;
} __attribute__((packed));

struct wave_sub_t {
	wave_str_t type;
	uint32_t size;
} __attribute__((packed));

struct wave_fmt_t {
	wave_sub_t sub;
	uint16_t format;
	uint16_t nch;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t align;
	uint16_t bpsample;
} __attribute__((packed));


/** Global variables **********************************************************/

/** Function prototypes *******************************************************/
static bool check_chunk(uint8_t* data, uint32_t len);
static bool process_fmt(wave_t* wave, wave_fmt_t* fmt);

/** Callback definitions ******************************************************/
static bool check_chunk(uint8_t* data, uint32_t len)
{

	if (len < sizeof(wave_chunk_t)) {
		return false;
	}

	wave_chunk_t* chunk = (wave_chunk_t*) data;

	if (chunk->riff.value != STR_RIFF) {
		return false;
	}

	if (chunk->wave.value != STR_WAVE) {
		return false;
	}

	return true;
}

static bool process_fmt(wave_t* wave, wave_fmt_t* fmt)
{

	if (fmt->format != FORMAT_PCM) {
		return false;
	}

	wave->channels = fmt->nch;
	wave->sample_rate = fmt->sample_rate;
	wave->byte_rate = fmt->byte_rate;
	wave->align = fmt->align;
	wave->bpsample = fmt->bpsample;

	return true;
}

/** Function definitions ******************************************************/

/** Public functions **********************************************************/
 err_t wave_init(wave_t* wave, uint8_t* buff, uint32_t len, uint16_t* next)
{
	uint32_t pos = 0;

	if (!check_chunk(buff, len)) {
		return ERR_INVALD_DATA;
	}

	pos += sizeof(wave_chunk_t);

	wave_sub_t* sub = (wave_sub_t*)(buff + pos);

	while (sub->type.value != STR_DATA) { 
		if (len - pos < sizeof(wave_sub_t)) {
			return ERR_INVALD_DATA;
		}

		if (sub->type.value == STR_FMT) {
			if (!process_fmt(wave, (wave_fmt_t*) sub)) {
				return ERR_INVALD_DATA;
			}
		}

		pos += sub->size +  sizeof(wave_sub_t);
		sub = (wave_sub_t*)(buff + pos);
	}

	wave->length = sub->size;
	wave->position = 0;
	pos += sizeof(wave_sub_t);
	*next = pos;

	return SUCCESS;
}

 uint16_t wave_dec(wave_t* wave, wave_io_t* io, uint16_t* next)
{
	uint16_t samples = io->in_len / wave->align;

	if (io->out_len < samples) {
		samples = io->out_len;
	}

	if (wave->bpsample != 8) {
		return ERR_NOT_SUPPORTED;
	}

	for (int i = 0; i < samples; i++) {
		io->out[i] = io->in[i * wave->align];
	}

	*next = samples * wave->align;

	return samples;
}
