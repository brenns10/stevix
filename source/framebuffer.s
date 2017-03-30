  .section .data
  .align 4
  .globl frame_buffer_info
frame_buffer_info:
  .int 1024 // #00 physical width
  .int 768  // #04 physical height
  .int 1024 // #08 virtual width
  .int 768  // #12 virtual height
  .int 0    // #16 gpu - pitch
  .int 16   // #20 bit depth
  .int 0    // #24 X
  .int 0    // #24 Y
  .int 0    // #32 gpu - pointer
  .int 0    // #36 gpu - size

  .align 4
  .globl font
font:
  .incbin "font.bin"

  .section .text

/*
  Ask GPU to initialize the frame buffer:
r0: width
r1: height
r2: bit depth
  Returns
r0: pointer to frame buffer info, or 0 on failure
*/
  .globl init_frame_buffer
init_frame_buffer:
  width .req r0
  height .req r1
  bit_depth .req r2
  cmp width, #4096
  cmpls height, #4096
  cmpls bit_depth, #32
  result .req r0
  movhi result, #0
  movhi pc, lr

  fbinfo .req r3
  ldr fbinfo, =frame_buffer_info
  str width, [fbinfo]
  str height, [fbinfo, #4]
  str width, [fbinfo, #8]
  str height, [fbinfo, #12]
  str bit_depth, [fbinfo, #20]
  .unreq width
  .unreq height
  .unreq bit_depth

  push {lr}
  push {fbinfo}
  mov r0, fbinfo
  add r0, #0x40000000
  mov r1, #1
  bl mailbox_write

  mov r0, #1
  bl mailbox_read
  pop {fbinfo}

  teq result, #0
  movne result, #0
  popne {pc}

  mov result, fbinfo
  pop {pc}
  .unreq result
  .unreq fbinfo
