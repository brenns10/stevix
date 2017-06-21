/**
 * pi.h: Contains all function declarations.
 *
 * This file is ordered into sections, where each section is implemented in the
 * same file. For documentation on a specific function, see the file in which it
 * is defined: there should be documentation above the definition.
 */
#ifndef _PI_H
#define _PI_H

/*
 * I prefer stdint types but I don't believe I can simply *use* them without the
 * compiler accidentally linking in some standard library. So here are the types
 * I'm using (explicitly).
 */
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef short int16_t;
typedef char int8_t;

/*
 * GPIO - defined in gpio.s
 * Can use this to set GPIO's to input/output, and then to turn them on/off.
 */
extern void set_gpio_function(uint32_t pin, uint32_t mode);
extern void set_gpio(uint32_t pin, uint32_t value);

/*
 * Timer - defined in timer.s
 * Currently the only timer related functions are wait()
 */
extern void wait(uint32_t time_lo, uint32_t time_hi);

/*
 * Postman - defined in postman.s
 * Manages reading and writing from mailboxes.
 */
extern void mailbox_write(uint32_t value, uint32_t channel);
extern uint32_t mailbox_read(uint32_t channel);

/*
 * Frame Buffer - defined in framebuffer.s
 * We use this to get a hold of a frame buffer which we can write in and have
 * the GPU draw it to the screen.
 */
struct fbinfo {
	uint32_t phys_width;
	uint32_t phys_height;
	uint32_t virt_width;
	uint32_t virt_height;
	uint32_t pitch;
	uint32_t bit_depth;
	uint32_t x;
	uint32_t y;
	void *ptr;
	uint32_t size;
};
extern struct fbinfo *init_frame_buffer(uint32_t width, uint32_t height,
                                        uint32_t color);

/*
 * Drawing - defined in drawing.c
 * We use this to draw into frame buffers. Note that drawing relies on two
 * static fields - fore_color and graphics_address. These ought to be set before
 * attempting to draw.
 */
struct draw {
	uint16_t fore_color;
	struct fbinfo *fbinfo;
	void (*brush)(struct draw *params, uint32_t x, uint32_t y);
};
extern void draw_pixel(struct draw *p, uint32_t x0, uint32_t y0);
extern void draw_line(struct draw *p, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

/*
 * Random - defined in random.s
 * A very quick and dirty pseudo random number generator.
 */
extern uint32_t random(uint32_t last);

/*
 * Text - draw text (text.c)
 * These function deal solely with drawing characters from a font, and then
 * laying out strings. For more complex console output, there is printf.
 */
void init_console(uint32_t width, uint32_t height);
int puts(char *string);

/*
 * Tags - parse tags (tags.c)
 * Reads "tags" from the 0x100 memory region containing things like the kernel
 * command line.
 */
struct tag {
	uint32_t num_words;
	uint16_t tag_number;
	uint16_t unused;
	uint8_t data[0];
};
struct tag *find_tag(uint16_t tag_number);

/*
 * Math - (math.s)
 * Provides division support, since this does not exist in ARM.
 */
extern uint32_t divide_u32(uint32_t dividend, uint32_t divisor);

/*
 * Strings - (string.s)
 * Functions that would belong in string.h
 */
void wordcpy(void *dest, void *src, uint32_t n);
void wordset(void *dest, uint32_t val, uint32_t n);

/*
 * Formatting! - (format.c)
 * For printing numbers and things to the console.
 */
uint32_t snprintf(char *buf, uint32_t size, const char *format, ...);
uint32_t printf(const char *format, ...);

/*
 * Serial - (serial.c)
 * For transmitting / receiving data along the serial (UART1)
 */
void init_serial(void);
void send_byte(uint8_t byte);
uint8_t recv_byte(void);

/*
 * Register definitions from datasheet. These are translated from their "bus"
 * addresses as per the datasheet.
 *
 * http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/downloads/SoC-Peripherals.pdf
 *
 * Referred to this example code for their values and basic setup of serial:
 *
 * https://github.com/dwelch67/raspberrypi/blob/master/uart01/uart01.c
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


#endif
