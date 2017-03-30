/**
 * text.c: contains functions for drawing text
 */
#include "pi.h"

#define CHEIGHT 16
#define LG_CHEIGHT 4
#define CWIDTH 8
#define LG_CWIDTH 3
#define DEPTH 2

extern uint8_t font[];

struct {
	uint32_t curs_x;
	uint32_t curs_y;
	struct draw draw;
} _console = {1}; // initializer is a hack to get it into .data section... :/

void _draw_character(char c)
{
	struct draw *d = &_console.draw;
	uint32_t y = _console.curs_y;
	uint32_t x = _console.curs_x;
	if (c > 127)
		return;
	uint32_t byte_addr = c << 4;
	do {
		uint8_t bits = font[byte_addr];
		int32_t bit = 0;
		while (bit < 8) {
			if (bits & 0x1)
				d->brush(d, x + bit, y);
			bit++;
			bits >>= 1;
		}
		y++;
		byte_addr++;
	} while ((byte_addr & 0xF) != 0);
}


uint32_t _compute_scroll(char *string)
{
	uint32_t x = _console.curs_x;
	uint32_t y = _console.curs_y;

	while (*string) {
		switch (*string) {
		case '\n':
			x = 0;
			y += CHEIGHT;
			break;
		case '\t':
			x += 4 * CWIDTH;
			break;
		default:
			x += CWIDTH;
			break;
		}
		if (x >= _console.draw.fbinfo->virt_width - CWIDTH) {
			x = 0;
			y += CHEIGHT;
		}
		string++;
	}
	if (y + CHEIGHT >= _console.draw.fbinfo->virt_height)
		return y + CHEIGHT - _console.draw.fbinfo->virt_height + 1;
	else
		return 0;
}

void _scroll(uint32_t lines)
{
	void *data = _console.draw.fbinfo->ptr;
	uint32_t gap = lines * DEPTH * _console.draw.fbinfo->virt_width;
	uint32_t amt = _console.draw.fbinfo->size - gap;
	wordcpy(data, data + gap, amt);
	wordset(data + amt, 0, gap);
	_console.curs_y -= lines;
}
void _put_string(char *string)
{
	struct draw *d = &_console.draw;
	while (*string) {
		_draw_character(*string);
		switch (*string) {
		case '\n':
			_console.curs_x = 0;
			_console.curs_y += CHEIGHT;
			break;
		case '\t':
			_console.curs_x += 4 * CWIDTH;
			break;
		default:
			_console.curs_x += CWIDTH;
			break;
		}
		if (_console.curs_x >= d->fbinfo->virt_width) {
			_console.curs_x = 0;
			_console.curs_y += CHEIGHT;
		}
		string++;
	}
}

void put_string(char *string)
{
	uint32_t scroll = _compute_scroll(string);
	if (scroll) _scroll(scroll);
	_put_string(string);
}

void init_console(uint32_t width, uint32_t height)
{
	_console.curs_x = 0;
	_console.curs_y = 0;
	_console.draw.fbinfo = init_frame_buffer(width, height, DEPTH);
	_console.draw.fore_color = 0xFFFF;
	_console.draw.brush = draw_pixel;
}
