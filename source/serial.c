/**
 * serial.c: contains serial interface code
 */
#include "pi.h"

#define set_u32(address, value) *(uint32_t *)address = (uint32_t)value
#define set_u16(address, value) *(uint16_t *)address = (uint16_t)value
#define set_u8(address, value) *(uint8_t *)address = (uint8_t)value

#define get_u32(address) (*(uint32_t*)address)
#define get_u16(address) (*(uint16_t*)address)
#define get_u8(address) (*(uint8_t*)address)

void init_serial(void)
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

#define TX_READY (get_u32(AUX_MU_LSR_REG) & (1 << 5))

void send_byte(uint8_t byte)
{
	/* wait until we can send a byte */
	while (!TX_READY);
	set_u8(AUX_MU_IO_REG, byte);
}

#define RX_READY (get_u32(AUX_MU_LSR_REG) & 1)

uint8_t recv_byte(void)
{
	while (!RX_READY);
	return get_u8(AUX_MU_IO_REG);
}
