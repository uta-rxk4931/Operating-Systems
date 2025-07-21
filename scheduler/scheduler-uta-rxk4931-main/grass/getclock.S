/*
 * (C) 2023, Trevor Bakker
 * All rights reserved.
 */

.global getclock
.type	getclock, %function

getclock:
  addi    sp,sp,-32
  rdtime t0         # Read the time from the clock register
  mv a0, t0 
  addi    sp,sp,32
  jr      ra

