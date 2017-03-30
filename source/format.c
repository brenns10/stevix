/**
 * format.c: provides sprintf, and printf as well
 */
#include <stdarg.h>
#include "pi.h"

/**
 * This function implements the %x format specifier.
 * @buf: Pointer to the output buffer.
 * @size: Size of the output buffer.
 * @out: Current index into the output buffer.
 * @val: Value to format as hexadecimal.
 * @return: New index into the output buffer.
 */
uint32_t _snprintf_hex(char *buf, uint32_t size, uint32_t out, uint32_t val)
{
	uint32_t mask = 0xF0000000;
	uint32_t shift = 32;
	uint32_t digit;
	uint32_t started = 0;

	do {
		shift -= 4;
		digit = (val & mask) >> shift;

		// If statement ensures we only print the rightmost digits.
		// I'm still debating if this is useful.
		if (digit || started || shift == 0) {
			started = 1;
			if (digit >= 10) {
				buf[out++] = 'a' + digit - 10;
			} else {
				buf[out++] = '0' + digit;
			}
		}
		mask >>= 4;
	} while (shift > 0);
	return out;
}

/**
 * This is the fundamental formatting function, although it is not the one users
 * will call frequently. The v means that it takes a va_list directly, which is
 * useful for sharing code across variadic functions. The sn means that it will
 * write to a buffer (the buffer length protection is not completely implemented
 * yet).
 *
 * Currently, there is only one format specifier, %x for hex unsigned 32 bit
 * integers. Eventually there will be support for %d, %u, and %s. I have no plan
 * to support the full range of format specification - things like specifying
 * width of fields and prefix characters.
 *
 * @buf: Where to write the output
 * @size: Size of the output buffer
 * @format: Format string
 * @vl: Argument list
 * @return: number of bytes written
 */
uint32_t vsnprintf(char *buf, uint32_t size, const char *format, va_list vl)
{
	uint32_t out = 0;
	uint32_t uintval;

	for (uint16_t in = 0; format[in]; in++) {
		if (format[in] == '%') {
			in++;
			if (!format[in]) {
				buf[out++] = '%';
				buf[out++] = '\0';
				return out;
			}
			switch (format[in]) {
			case 'x':
				uintval = va_arg(vl, uint32_t);
				out = _snprintf_hex(buf, size, out, uintval);
				break;
			default:
				buf[out++] = '%';
				buf[out++] = format[in];
			}

		} else {
			buf[out++] = format[in];
		}
	}
	buf[out++] = '\0';
	return out;
}

/**
 * Format a string into a buffer, without exceeding its size. See vsnprintf()
 * for details on formatting.
 * @buf: Where to write the output
 * @size: Size of the output buffer
 * @format: Format string
 * @return: Number of bytes written
 */
uint32_t snprintf(char *buf, uint32_t size, const char *format, ...)
{
	uint32_t res;
	va_list vl;
	va_start(vl, format);
	res = vsnprintf(buf, size, format, vl);
	va_end(vl);
	return res;
}

/**
 * Format a string and print it to the console. See vsnprintf() for details on
 * formatting.
 *
 * NOTE: There is a fixed buffer size (see below). Make sure your messages will
 * fit into it. Also, the buffer is stack-allocated, so we need to be careful
 * with the size, or we may start running into the TAGS section.
 *
 * @format: Format string
 * @return: Number of bytes written
 */
uint32_t printf(const char *format, ...)
{
	char buf[512];
	uint32_t res;
	va_list vl;
	va_start(vl, format);
	res = vsnprintf(buf, sizeof(buf), format, vl);
	va_end(vl);
	puts(buf);
	return res;
}
