Setup
=====

This document describes how to get Stevix running. Currently it's not actually
even an OS, so if you are trying to set it up you must be really curious.

Prerequisites
-------------

(Board related, that is.)

- Raspberry Pi 1, Model B. This is the only hardware I have to develop on. It is
  likely that most things won't work properly on other boards. I'd be interested
  in supporting more, but that's out of scope for me right now.
- USB Serial cable. I use [this][usb-serial]. You can find information on other
  types of serial cable, and which wires to connect where, at [this][dwelch67]
  repository.
- SD card. Should have MBR partition table, 1 partition of type C. Partition
  should be formatted to FAT32.
- Firmware files from [here][rpi-firmware]. You should have the following files
  in the root of your SD card:
  - `bootcode.bin`
  - `start*.elf` - make sure to include the `_x`, `_cd`, variants (see Gotchas)
  - `fixup*.dat` - again, make sure to include all variants
- Config file copied from this repo into the SD card root: `config.txt`

Configure
---------

There's not much to configure.

You can select which UART you'd like to use for serial output. See
`source/serial.h`.

Compile
-------

You'll need a Linux machine, or else something that can run a GCC
cross-compiler. I'm using `gcc-arm-none-eabi` from Arch Linux, I believe a
similar cross compiler exists in Debian/Ubuntu world.

Compile should be a simple `make`.

Install
-------

"Install" just means copying your `kernel.img` onto the SD card. The "SD card
dance" is the following commands, which will inevitably be in your bash history,
so it's just a combination of up arrow and enter:

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

Run
---

Before powering up, put the SD card in, and connect the serial cable to the Pi
and your computer. Start your serial terminal emulator (picocom recommended)
before turning on the Pi.

Once those things are done, turn on power. Output should scroll from the
console, and you'll end up with a REPL.

[usb-serial]: https://www.amazon.com/dp/B00QT7LQ88/ref=cm_sw_r_cp_dp_T1_86QszbEMXM81N
[dwelch67]: https://github.com/dwelch67/raspberrypi
[rpi-firmware]: https://github.com/raspberrypi/firmware
