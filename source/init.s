/*
  Welcome! This is where it all starts. After initialization the GPU (ugh) loads
  kernel.img and sets pc to 0x8000. Here we set up the stack pointer and invoke
  our main() function. If the main function ever returns, we enter an infinite
  loop.
*/
  .section .init
  .globl _start
_start:
  mov sp, #0x8000
  bl main
$loop:
  b $loop
