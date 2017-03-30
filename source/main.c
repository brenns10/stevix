/**
 * main.c: contains the main() function
 */
#include "pi.h"

/**
 * "Handles" errors by turning on the ACT led, and then looping forever.
 */
void error(void) {
	set_gpio(16, 0);
	while (1) {
		// infinite loop
	}
}

/**
 * Main entry point of the program - called by _start in init.s.
 *
 * This program pseudo-randomly draws lines across the screen.
 */
int main() {
	set_gpio_function(16, 1);
	init_console(1920, 1080);
	put_string("hello, world\n");
	put_string("this is my first use ");
	put_string("of the console.\n");
	put_string("it is not likely to be successful.\n");

	while (1) {
		put_string("Now I will alternate between...\n");
		wait(250000, 0);
		put_string("These two lines so I can test scroll...\n");
		wait(250000, 0);
	}
}
