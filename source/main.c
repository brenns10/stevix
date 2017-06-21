/**
 * main.c: contains the main() function
 */
#include "pi.h"
#include "serial.h"

#define MILLIS * 1000
#define SECS * 1000 MILLIS

/**
 * Blink the ACT LED with a given time interval.
 */
void blink(uint32_t interval) {
	while (1) {
		set_gpio(16, 0);
		wait(interval, 0);
		set_gpio(16, 1);
		wait(interval, 0);
	}
}

/**
 * "Handles" errors by blinking the ACT LED very quickly.
 */
void error(void) {
	blink(50 MILLIS);
}

/**
 * Constantly send the alphabet.
 */
void alpha(void)
{
	uint8_t ch;
	for (;;)
		for (ch = 'A'; ch <= 'Z'; ch++)
			serial_send(ch);
}

/**
 * Main entry point of the program - called by _start in init.s. It is safe to
 * return from main: the _start stub will infinitely loop on return.
 */
int main() {
	struct tag *tag = 0;
	char buffer[512];

	/* turns on the led */
	set_gpio_function(16, 1);
	set_gpio(16, 0);

	serial_init();

	printf("Stevix version %u.%u.%u (GCC %s)\n", MAJOR, MINOR, PATCH,
	       __VERSION__);
	tag = find_tag(9);
	if (tag) {
		puts("Command Line: ");
		puts((char*)tag->data);
		puts("\n");
	} else {
		puts("No command line found???\n");
	}

	puts("\nStevix shell. Configure your terminal program:\n"
	     "1. Expect CRLF line endings on output.\n"
	     "2. Send CR when Enter key pressed.\n"
	     "3. Enable local echo.\n\n");

	for (;;) {
		puts("> "); /* printf with format string length 1 -> putchar */
		serial_recv_line(buffer, sizeof(buffer));
		printf("Your input: \"%s\"\n", buffer);
	}
}
