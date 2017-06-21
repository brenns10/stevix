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
	set_gpio_function(16, 1);
	set_gpio(16, 0);
	serial_init();
	alpha();
}
