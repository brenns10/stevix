/**
 * drawing.c: Contains utilities for drawing to a framebuffer.
 */
#include "pi.h"

void draw_pixel(struct draw *p, uint32_t x, uint32_t y)
{
	if (x >= p->fbinfo->virt_width || y >= p->fbinfo->virt_height)
		return;
	uint16_t *buffer = p->fbinfo->ptr;
	buffer[y * p->fbinfo->virt_width + x] = p->fore_color;
}

void draw_line(struct draw *p, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
	uint32_t dx, dy;
	int32_t sx, sy, err;

	if (x1 > x0) {
		dx = x1 - x0;
		sx = 1;
	} else {
		dx = x0 - x1;
		sx = -1;
	}

	if (y1 > y0) {
		dy = y1 - y0;
		sy = 1;
	} else {
		dy = y0 - y1;
		sy = -1;
	}

	err = dx - dy;
	while (x0 != x1 + sx && y0 != y1 + sy) {
		p->brush(p, x0, y0);
		if (err * 2 >= -(int32_t)dy) {
			x0 += sx;
			err -= dy;
		}
		if (err * 2 <= (int32_t)dx) {
			y0 += sy;
			err += dx;
		}
	}
}
