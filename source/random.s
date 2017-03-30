  .globl random
/*
  Completely unmodified prng from Baking Pi.
r0: Previous random number.
Returns:
r0: Next random number.
*/
random:
  xnm .req r0
  a .req r1

  mov a, #0xEF00
  mul a, xnm
  mul a, xnm
  add a, xnm
  .unreq xnm
  add r0, a, #73
  .unreq a
  mov pc, lr
