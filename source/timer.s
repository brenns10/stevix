/*
  Wait a specified number of microseconds (approximately). This will work for
  wait times which are more than 4 bytes. Unfortunately, this implementation
  will begin to fail once the combined time since boot plus wait time will cause
  an 8 byte wrap around. Thankfully, this combined time would have to exceed a
  bit over 585,942 years, and so I'm not concerned about it now.
r0: the number of microseconds to wait, low 4 bytes
r1: the number of microseconds to wait, high 4 bytes
*/
  .globl wait
wait:
  push {r4,r5}
  wait_until_lo .req r0
  wait_until_hi .req r1
  timer_value_lo .req r2
  timer_value_hi .req r3

  // put the timer values in their correct locations
  timer_address .req r4
  ldr timer_address, =0x20003000
  ldrd timer_value_lo, timer_value_hi, [timer_address, #4]

  // add timer values to the wait time to get the "wait until" time
  overflow .req r5
  mov overflow, #0
  adds wait_until_lo, wait_until_lo, timer_value_lo
  movcs overflow, #1
  add wait_until_hi, wait_until_hi, overflow
  .unreq overflow
  add wait_until_hi, wait_until_hi, timer_value_hi

  // loop checking the timer, and break out once the timer exceeds the wait
  // until time
$wait_loop:
  ldrd timer_value_lo, timer_value_hi, [timer_address, #4]
  cmp timer_value_lo, wait_until_lo
  cmphs timer_value_hi, wait_until_hi
  bhs $wait_exit
  blo $wait_loop

$wait_exit:
  .unreq wait_until_lo
  .unreq wait_until_hi
  .unreq timer_value_lo
  .unreq timer_value_hi
  .unreq timer_address
  pop {r4,r5}
  mov pc, lr
