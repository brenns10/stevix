/**
 * serial.h: contains the API for accessing the serial port
 */
#ifndef _SERIAL_H
#define _SERIAL_H

#include "pi.h"

/*
 * Registers for the Mini UART. I've had some serious trouble getting Serial on
 * Mini UART to work consistently. However, the code lives on.
 */
#define AUXENB          0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

/*
 * Registers for the PL011 UART.
 */
#define PL011_DR     0x20201000
#define PL011_RSRECR 0x20201004
#define PL011_FR     0x20201018
#define PL011_ILPR   0x20201020
#define PL011_IBRD   0x20201024
#define PL011_FBRD   0x20201028
#define PL011_LCRH   0x2020102C
#define PL011_CR     0x20201030
#define PL011_IFLS   0x20201034
#define PL011_IMSC   0x20201038
#define PL011_RIS    0x2020103C
#define PL011_MIS    0x20201040
#define PL011_ICR    0x20201044
#define PL011_DMACR  0x20201048
#define PL011_ITCR   0x20201080
#define PL011_ITIP   0x20201084
#define PL011_ITOP   0x20201088
#define PL011_TDR    0x2020108C

/*
 * Some convenient register access functions.
 */
#define set_u32(address, value) *(uint32_t *)address = (uint32_t)value
#define set_u16(address, value) *(uint16_t *)address = (uint16_t)value
#define set_u8(address, value) *(uint8_t *)address = (uint8_t)value
#define get_u32(address) (*(uint32_t*)address)
#define get_u16(address) (*(uint16_t*)address)
#define get_u8(address) (*(uint8_t*)address)

/*
 * Function declarations. You should probably use the #defines in the section
 * below, so that you can toggle the serial implementation.
 */
void serial_miniuart_init(void);
void serial_pl011_init(void);
void serial_send_buffer(uint8_t *buffer, uint32_t nbytes);
void serial_puts(char *string);
void serial_recv_buffer(uint8_t *buffer, uint32_t nbytes);
void serial_recv_line(char *buffer, uint32_t nbytes);

/* select which UART for serial */
#define SERIAL_PL011

#if defined(SERIAL_PL011) /* implementations for PL011 serial *****************/

#define serial_init serial_pl011_init
#define serial_tx_ready() (!(get_u32(PL011_FR) & (1 << 5))) /* not TXFE */
#define serial_rx_ready() (!(get_u32(PL011_FR) & (1 << 4))) /* not RXFE */
#define serial_try_send(byte) set_u8(PL011_DR, byte)
#define serial_try_recv(byte) get_u8(PL011_DR)

#elif defined(SERIAL_MINI_UART) /* implementations for Mini UART **************/

#define serial_init serial_miniuart_init
#define serial_tx_ready() (get_u32(AUX_MU_LSR_REG) & (1 << 5))
#define serial_rx_ready() (get_u32(AUX_MU_LSR_REG) & 1)
#define serial_try_send(byte) set_u8(AUX_MU_IO_REG, byte)
#define serial_try_recv(byte) get_u8(AUX_MU_IO_REG)

#else /* else *****************************************************************/

#error "no serial device selected"

#endif /* endif ***************************************************************/

static inline void serial_send(uint8_t byte)
{
	while (!serial_tx_ready());
	serial_try_send(byte);
}

static inline uint8_t serial_recv(void)
{
	while (!serial_rx_ready());
	return serial_try_recv();
}

#endif//_SERIAL_H
