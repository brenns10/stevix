/**
 * serial.c: contains serial interface code
 */
#include "serial.h"

void serial_miniuart_init(void)
{
	/* set function of GPIO 14, 15 to be alternative 5 (Mini UART) */
	set_gpio_function(14, 2);
	set_gpio_function(15, 2);

	/* first we must enable the Mini UART in order to access registers */
	set_u32(AUXENB, 1);

	/* this implementation is not interrupt driven, so disable them */
	set_u32(AUX_MU_IER_REG, 0);

	/* 8-bit format. According to dwelch67's uart01 example, we need bit 1
	 * set to enable 8 bit, not bit 0.
	 */
	set_u32(AUX_MU_LCR_REG, 3);

	/* writing zero clears rx/tx fifo */
	set_u32(AUX_MU_IIR_REG, 0);

	/* baud rate - 115200 (ish) ~= 250000000 / (8 * (270 + 1)) */
	set_u32(AUX_MU_BAUD_REG, 270);

	/* finally, enable rx and tx */
	set_u32(AUX_MU_CNTL_REG, 2);
}

void serial_pl011_init(void)
{
	/* set function of GPIO 14, 15 to alternative 0 (PL011 UART) */
	set_gpio_function(14, 4);
	set_gpio_function(14, 4);

	/* word length = 8bit, enable fifo, no parity, etc */
	set_u32(PL011_LCRH, 0x70);

	/* set baud rate to 115200
	 * According to BCM2835 Peripherals datasheet, this is the formula for
	 * the baud rate divisor:
	 *   UART Reference Clock / (16 * 115200) = 1.6276
	 * YOU MUST SET THESE THINGS IN config.txt!
	 * With that set, the ibrd and fbrd can be computed:
	 *   integer part = 1
	 * According to "ARM PrimeCell UART (PL011) Revision: r1p5 Technical
	 * Reference Manual" page 2-11, the fractional part should be 6 bits, or
	 * out of 64. So:
	 *   fractional part = 40 (ish)
	 */
	set_u32(PL011_IBRD, 1);
	set_u32(PL011_FBRD, 40);

	/* mask out all the interrupts */
	set_u32(PL011_IMSC, 0x7F2);

	/* enable rx, tx, and of course enable UART0 */
	set_u32(PL011_CR, 0x301);
}

/**
 * Send a whole buffer of bytes.
 */
void serial_send_buffer(uint8_t *buffer, uint32_t nbytes)
{
	uint32_t i;
	for (i = 0; i < nbytes; i++) {
		serial_send(buffer[i]);
	}
}

/**
 * Receive nbytes from serial, placing them into buffer.
 */
void serial_recv_buffer(uint8_t *buffer, uint32_t nbytes)
{
	uint32_t i;
	for (i = 0; i < nbytes; i++) {
		buffer[i] = serial_recv();
	}
}

/**
 * Receive a line into a user-provided buffer. Returns once CRLF has been read,
 * or when the buffer has become full.
 */
void serial_recv_line(char *buffer, uint32_t nbytes)
{
	uint32_t i;
	for (i = 0; i < nbytes - 1; i++) {
		buffer[i] = serial_recv();
		if (buffer[i] == '\r') {
			serial_recv(); // probably going to be \n :P
			buffer[i] = '\0';
			return;
		}
	}
	buffer[i] = '\0'; // nul-terminate just in case
	return;
}
