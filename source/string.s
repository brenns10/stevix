/*
  Copies n bytes from src to dest, assuming they don't overlap. This is the most
  naive implementation, but yolo, right? This goes word by word, so your args
  ought to be word aligned.
r0: dest
r1: src
r2: n
*/
  .globl wordcpy
wordcpy:
  dest .req r0
  src .req r1
  n .req r2

  ldr r3, [src], #4 // side effect: increment src by 4
  str r3, [dest], #4
  subs n, #4
  bge wordcpy
  mov pc, lr

  .unreq dest
  .unreq src
  .unreq n

/*
  Set the words pointed by dest to have value val, for n bytes. The value is a
  word, and so the addresses (and n) have to be word aligned.
r0: dest
r1: val
r2: n
*/
  .globl wordset
wordset:
  dest .req r0
  val .req r1
  n .req r2

  str val, [dest], #4
  subs n, #4
  bge wordset
  mov pc, lr

  .unreq dest
  .unreq val
  .unreq n
