/*
  Division. Currently, this doesn't seem to work. Will revisit.
r0: dividend
r1: divisor
  returns
r0: result
*/
  .globl divide_u32
divide_u32:
  result .req r0
  remainder .req r1
  shift .req r2
  current .req r3

  clz shift, r1
  clz r3, r0
  sub shift, shift, r3
  lsl current, r1, shift
  mov remainder, r0
  mov result, #0

$divide_u32_loop:
  cmp shift, #0
  blt $divide_u32_return
  cmp remainder, current
  addge result, result, #1
  subge remainder, current
  sub shift, #1
  lsr current, #1
  lsl result, #1
  b $divide_u32_loop

$divide_u32_return:
  .unreq current
  mov pc, lr
  .unreq result
  .unreq remainder
  .unreq shift

/*
  Calls to these functions are emitted by the C compiler for the / operator and
  the % operator. I have implemented them (they would normally be included in
  a standard library of sorts) for my own convenience.
*/
  .globl __aeabi_uidiv
__aeabi_uidiv:
  b divide_u32

  .globl __aeabi_uimod
__aeabi_uimod:
  push {pc}
  bl divide_u32
  mov r0, r1
  pop {lr}
