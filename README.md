stevix*
=======

*\*it doesn't work as well with my name as it did with Linus*

This is a long term project in which I hope to build a simple operating system
for the Raspberry Pi.

Setup
-----

Prereqs:

- Raspberry Pi 1, Model B. It may work on other models, with some effort.
- USB serial cable. I use [this][usb-serial]. See [here][dwelch67] for info on
  cables and how to connect.
- An SD card, with MBR partition table, 1 partition (type C), formatted to
  FAT32.
- Firmware from [here][rpi-firmware]: `bootcode.bin` and `start.elf`
- A config file (`config.txt`), copied from this repo

With all of those prereqs, you can now do:

```bash
# Build kernel.img
$ make

# Mount the filesystem (replace sdc1 with your SD partition)
$ sudo mount /dev/sdc1 /mnt

# Copy kernel.img
$ sudo cp kernel.img /mnt

# Unmount
$ sudo umount /mnt
```

Next, put the SD card in the Raspberry Pi. Plug the serial cable into your
computer and the Pi, and start `picocom`:

```bash
$ picocom -c -b 115200 /dev/ttyUSB0
```

Connect the Pi to power and you should see the version, command line, and a
silly REPL!

Status
------

An OS is a huge undertaking, and there are lots of milestones to reach before
you can really claim to have an OS. Here are the things I currently have
accomplished.

- [x] Simple HDMI framebuffer, with line drawing
- [x] Framebuffer console output
- [x] Serial input/output, on both the Mini UART and PL011 UART
- [x] Simple printf formatting for either serial or framebuffer

Some things that are on the horizon:

- [ ] Support more format specifiers in string formatting (not a very high
  priority right now)
- [ ] Process abstraction, with time slices and scheduling. No memory
  protections yet. Separate stack.
- [ ] Memory segmentation
- [ ] USB keyboard input (a major undertaking)
- [ ] FAT32 filesystem implementation

This would get me to my first milestone. Here is the current set of milestones
that I have in mind:

1. A minimal shell that supports keyboard input. Basic utilities to explore the
   file system (probably read-only). Future features will be exposed as commands
   until I have the ability to create an init process.
2. The ability to enter "user-mode" and run something. This will require me to
   figure out the MMU and how I will manage memory for processes (even though
   process won't actually exist yet).
3. A real "process" abstraction, complete with a system call interface and a few
   system calls. At this point, the process code will likely be copied from the
   kernel image into the user-mode memory space, rather than trying to support
   ELF.
4. A real "file" abstraction, with an implementation that supports the console.
   At this point, a process should be able to read and write to the console as
   its "stdin" and "stdout".
5. Some sort of scheduling system (brain-dead simple first). The important thing
   is to be able to set time slices, run the process in a time slice, and then
   transition back into kernel-mode in order to schedule something else.
6. Some form of interrupt handling that respects processes. This way, the USB
   driver could use interrupts to get input.
7. A real user-mode shell as init!
8. Process management system calls so that you can create a new process.
9. ELF support? Some sort of format for storing executables.

Like I said, this is a truly long-term project.

License, Credits
----------------

There are a lot of great resources that have helped me:
- [Baking Pi][]
- [dwelch67's collection of bare-metal examples][dwelch67]
- [BCM2835 perpiherals datasheet](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/downloads/SoC-Peripherals.pdf)
- [PrimeCell UART (PL011) datasheet](http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183g/DDI0183G_uart_pl011_r1p5_trm.pdf)

I'd like to release this under Revised BSD, as per usual.

[Baking Pi]: http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/index.html
[rpi-firmware]: https://github.com/raspberrypi/firmware
[rpi-config]: https://www.raspberrypi.org/documentation/configuration/config-txt/README.md
[usb-serial]: https://www.amazon.com/dp/B00QT7LQ88/ref=cm_sw_r_cp_dp_T1_86QszbEMXM81N
[dwelch67]: https://github.com/dwelch67/raspberrypi
