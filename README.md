stevix*
=======

*\*it doesn't work as well with my name as it did with Linus*

A **very** long term project: build a simple operating system for a Raspberry
Pi. Inspired in part by TempleOS. My initial resource is Alex
Chadwick's [Baking Pi][] article series.

The Long Description
--------------------

I'm fascinated with how things work, and once I know how one thing works, I just
want to learn the next lower level of abstraction. Since I've been doing
OS-level work recently, operating systems have been on my mind. Recent news
brought TempleOS back to my attention, and inspired me to determine how I could
make my own minimal operating system.

The trouble is that making normal PC OS's has a huge scope. There are so very
many devices you would have to support, and so much "IBM PC" legacy to deal
with. I don't particularly care to spend all of my time on that - I'd rather
support a limited set of devices on one platform, so I can focus on what matters
to me: implementing the parts of an operating system.

Enter the Raspberry Pi. It has a standard set of hardware, it is available on
the cheap. It also uses ARM, a RISC architecture which is attractive to me. So
the Raspberry Pi provides the limited hardware scope I'm looking for in this
project.

Setup
-----

I'm using a Raspberry Pi 1, Model B. I would expect that things won't work quite
the same on other models (especially things like GPIO - you'll probably need to
alter some pins and memory addresses).

You'll need a SD card. The partition table should be MBR, and it should have one
primary partition formatted to FAT32 (the partition type should be C, "W95 Fat32
(LBA)"). The filesystem should contain at least: `bootcode.bin` and `start.elf`,
which are necessary firmware that you can get from [here][rpi-firmware] (`boot`
directory). You'll also want a `config.txt` file, where you can specify to the
firmware some settings (for me, I disabled HDMI overscan). [Here][rpi-config] is
some documentation on that file and what it can contain.

Your computer needs an ARM GCC cross compiler. Mine is `arm-none-eabi-gcc` on
Arch Linux.

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

Take your SD card, put it into the Raspberry Pi, and connect it to power and a
screen. You'll see a welcome message printed, and you'll get an LED blinking
once per second. Tada!

License
-------

Some of this code is heavily based on [Baking Pi][], and this is released under
a CC-BY-SA license. As a result, I believe this must also be CC-BY-SA. At some
point I may check with the author to see if I can't do GPL or something.

[Baking Pi]: http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/index.html
[rpi-firmware]: https://github.com/raspberrypi/firmware
[rpi-config]: https://www.raspberrypi.org/documentation/configuration/config-txt/README.md
