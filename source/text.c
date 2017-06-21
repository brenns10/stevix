/**
 * text.c: contains functions for drawing text
 *
 * This file contains several major pieces that come together to give us a nice
 * and simple console interface.
 * 1. Character drawing from a font.
 * 2. A stateful console that remembers the write position so you don't have to.
 * 3. Scrolling of the console when it reaches capacity.
 */
#include "pi.h"

#define CHEIGHT 16
#define LG_CHEIGHT 4
#define CWIDTH 8
#define LG_CWIDTH 3
#define DEPTH 2

/**
 * This is included in framebuffer.s - it has the contents of "font.bin"
 */
extern uint8_t font[];

/**
 * The console "state" goes here. We store pixel coordinates (not row/column) as
 * our position. Rather than buffering text, we simply scroll the framebuffer.
 */
struct {
	uint32_t curs_x;
	uint32_t curs_y;
	struct draw draw;
} _console = {1}; // initializer is a hack to get it into .data section... :/

/**
 * Draw a character in the basic ASCII set. The character is drawn to the
 * current location on the console, and the location is NOT updated. This is
 * heavily inspired from the Baking Pi tutorial, although it is implemented in
 * C, not assembly.
 * @c: character to draw
 */
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

/**
 * Compute the number of lines (of pixels, not text) that we will need to scroll
 * in order to print string. This basically goes through all the same steps as
 * printing a string, but without drawing it.
 * @string: text we want to print
 * @return: number of lines of pixels to scroll
 */
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
	/*
	 * Think of it this way: the bottom of the character is y + CHEIGHT. We
	 * want to be sure that y + CHEIGHT is on the bottom row of pixels,
	 * which is equal to height - 1. From this, the following condition and
	 * calculation should make sense.
	 */
	if (y + CHEIGHT >= _console.draw.fbinfo->virt_height)
		return y + CHEIGHT - _console.draw.fbinfo->virt_height + 1;
	else
		return 0;
}

/**
 * Actually scroll the frame buffer a certain number of lines. This implicitly
 * assumes that your background color is 0x0000, aka black. But it could be
 * changed trivially.
 *
 * Scrolling is implemented by simply moving the blocks of memory in the frame
 * buffer. This isn't efficient at all. At 1920x1080x2, that's a lot of bytes
 * being copied, especially by my mediocre wordcpy functions. An alternative
 * implementation might be to buffer all text on the console, then redraw it
 * with the background color, and then simply update a "head" pointer to the new
 * top of the console in the buffer. That is a significantly more complex
 * solution, in terms of code volume. So I have implemented the simple one.
 * Maybe I will revisit if performance becomes a problem.
 *
 * NOTE: this function uses word(cpy|set), two functions which assume that your
 *   framebuffer is word aligned. This almost certainly has to be the case (the
 *   GPU would be very dumb to allocate a non-word aligned frame buffer). But I
 *   can't say that I've seen any documentation that confirms it.
 *
 * @lines: number of pixel rows to scroll
 */
void _scroll(uint32_t lines)
{
	void *data = _console.draw.fbinfo->ptr;
	uint32_t gap = lines * DEPTH * _console.draw.fbinfo->virt_width;
	uint32_t amt = _console.draw.fbinfo->size - gap;
	wordcpy(data, data + gap, amt);
	wordset(data + amt, 0, gap);
	_console.curs_y -= lines;
}

/**
 * Print a string onto the console. This assumes that we have enough space, and
 * simply handles drawing the characters and advancing the cursor properly.
 * @string: the string to draw onto the console
 */
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

/**
 * Print a string to the console. This will scroll the console if necessary. It
 * is the main public interface of the text.c module.
 */
int console_puts(char *string)
{
	uint32_t scroll = _compute_scroll(string);
	if (scroll) _scroll(scroll);
	_put_string(string);
	return 0;
}

/**
 * Initialize the console. This is useful in order to streamline the startup
 * process. It handles creating the line drawing struct and initializing the
 * frame buffer with the GPU. However, as a result, the console basically OWNS
 * the frame buffer. You can't really draw to it.
 * @width: pixel width we would like our screen to be
 * @height: height we would like our screen to be
 */
void init_console(uint32_t width, uint32_t height)
{
	_console.curs_x = 0;
	_console.curs_y = 0;
	_console.draw.fbinfo = init_frame_buffer(width, height, DEPTH);
	_console.draw.fore_color = 0xFFFF;
	_console.draw.brush = draw_pixel;
}
