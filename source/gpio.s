/*
  Loads the address of the gpio controller into r0.
  No arguments.
*/
  .globl get_gpio_address
get_gpio_address:
  ldr r0, =0x20200000
  mov pc, lr

/*
  Set the GPIO function (input, output, or something else) for a given pin.
  r0: the GPIO pin to set the function of
  r1: the function to set it to
*/
  .globl set_gpio_function
set_gpio_function:
  // Check arguments
  cmp r0, #53
  cmpls r1, #7
  movhi pc, lr
  // Get GPIO address...
  push {lr}
  mov r2, r0
  bl get_gpio_address
  // Increment GPIO address until we find the right address for this pin.
function_loop$:
  cmp r2, #9
  subhi r2, #10
  addhi r0, #4
  bhi function_loop$
  // Shift the function and mask into the proper location
  mov r3, #7
  add r2, r2,lsl #1
  lsl r1, r2
  lsl r3, r2
  // Load the original GPIO settings and clear the bits we are working on.
  ldr r2, [r0]
  mvns r3, r3
  and r2, r2, r3
  // Finally, or the old settings with the updated settings for our PIN.
  orr r1, r1, r2
  // And store it.
  str r1, [r0]
  pop {pc}

/*
  Set the GPIO value for a pin.
r0: pin number
r1: pin value (0 or 1)
*/
  .globl set_gpio
set_gpio:
  pin_num .req r0
  pin_val .req r1
  // check pin_number argument
  cmp pin_num, #53
  movhi pc, lr

  // get the gpio address
  push {lr}
  mov r2, pin_num // shuffle :)
  .unreq pin_num
  pin_num .req r2
  bl get_gpio_address
  gpio_addr .req r0

  // the bit we need to set could be in one of two locations
  pin_bank .req r3
  lsr pin_bank, pin_num, #5
  lsl pin_bank, #2
  add gpio_addr, pin_bank
  .unreq pin_bank

  // compute the bit we are setting
  and pin_num, #31
  set_bit .req r3
  mov set_bit, #1
  lsl set_bit, pin_num
  .unreq pin_num

  // now we set a bit in the required location depending on whether we want to
  // turn it on or off
  teq pin_val, #0
  .unreq pin_val
  streq set_bit, [gpio_addr, #40]
  strne set_bit, [gpio_addr, #28]
  .unreq set_bit
  .unreq gpio_addr
  pop {pc}
