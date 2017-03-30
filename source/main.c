/**
 * main.c: contains the main() function
 */
#include "pi.h"

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
	blink(50000);
}

/**
 * Main entry point of the program - called by _start in init.s. It is safe to
 * return from main: the _start stub will infinitely loop on return.
 */
int main() {
	struct tag *tag = 0;
	set_gpio_function(16, 1);
	init_console(1920, 1080);

	puts("Welcome to Stevix, by Stephen Brennan\n");

	tag = find_tag(9);
	if (tag) {
		puts("Boot command line: ");
		puts((char*)tag->data);
		puts("\n");
	}

	printf("main = 0x%x\n", main);
	blink(500000); // slow blink is not error
}
