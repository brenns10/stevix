/*
  Return the address of the mailbox controller.
*/
  .globl get_mailbox_base
get_mailbox_base:
  ldr r0, =0x2000B880
  mov pc, lr

/*
  Write a value to a mailbox.
r0: value to write (28 bits)
r1: channel to write into (4 bits)
*/
  .globl mailbox_write
mailbox_write:
  tst r0, #0b1111
  movne pc, lr // low 4 bits must be empty
  cmp r1, #15
  movhi pc, lr // r1 <= 15

  push {lr}
  value .req r2
  channel .req r1
  mov value, r0
  bl get_mailbox_base
  mailbox .req r0

$mailbox_write_loop:
  status .req r3
  ldr status, [mailbox, #0x18]
  tst status, #0x80000000
  .unreq status
  bne $mailbox_write_loop

  orr value, channel
  .unreq channel
  str value, [mailbox, #0x20]
  .unreq value
  .unreq mailbox

  pop {pc}


/*
  Read a value from a mailbox.
r0: mailbox to read from (4 bits)
Returns:
r0: mailbox value
*/
  .globl mailbox_read
mailbox_read:
  cmp r0, #15
  movhi pc, lr

  push {lr}
  channel .req r1
  mov channel, r0
  bl get_mailbox_base
  mailbox .req r0

$mailbox_read_loop:
  status .req r2
  ldr status, [mailbox, #0x18]
  tst status, #0x40000000
  .unreq status
  bne $mailbox_read_loop

  read .req r2
  ldr read, [mailbox]
  .unreq mailbox
  inchan .req r3
  and inchan, read, #0b1111
  teq inchan, channel
  .unreq channel
  .unreq inchan
  bne $mailbox_read_loop

  and r0, read, #0xFFFFFFF0
  .unreq read
  pop {pc}
