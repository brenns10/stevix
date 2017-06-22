Gotchas
=======

This document aims to collect the pain points I've encountered, in the hopes
that future people may find it and not have to deal with them.

Boot Files
----------

Many resources I've looked at say that you only need to have `bootcode.bin` and
`start.elf` firmware files in your SD card. Older references may even describe
an additional `.bin` file. These resources are either outdated, or recent but
parroting outdated (and now wrong) information.

The `fixup*.dat` files contain information so that the `start*.elf` loaders may
move code around in memory. They are necessary in order to accurately report the
amount of memory your Pi has. Without them, my Pi 1 B shows 256MB of memory in
the boot command line, when it should show 512MB.

The variants of the boot files (`_x`, `_cd`, `_db`) are also important.
- `_x` - extended (probably more feature rich), requires more GPU memory
- `_cd` - cut down (allows smaller GPU memory footprint, allowing 16MB GPU mem)
- `_db` - debug

Since I need to make memory mappings, and I'd like to have plenty of free memory
available, I set the GPU to the lowest amount of memory, and I need all the
support files in order to allow this.

You can tell what memory the GPU has provisioned by looking at boot command
line. Mine has the following:

    vc_mem.mem_base=0x1fa00000
    vc_mem.mem_size=0x20000000
    
I interpret that to be `0x20000000` bytes of available SDRAM (i.e. 512MB) and
`0x1fa00000` is the point that the GPU starts its own "reserved" area of memory.
