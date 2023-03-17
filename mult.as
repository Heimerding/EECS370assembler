entry   lw 0 2 iter	   we start by loading in sixteen to register 2 - we'll be decrementing through this register
	    lw 0 3 mcand   then we load in mcand
	    lw 0 4 mplier  then we load in mplier
	    lw 0 5 one	   then we load in one
	    nor 0 1 6	   and we make a -1 real quick
start   beq 0 2 end	   loop start - will stop when all operations cease
	    nor 4 4 4	   the first step of the AND operation is to nor the first register with itself
	    nor 5 5 5	   the second step of the AND operation is to nor the second register with itself
	    nor 4 5 4	   to complete the AND operation we'll nor the first and second register together, put the result in mplier
	    beq 0 4 skip   if the result of the AND operation is zero, we'll skip the operation where we add the current register to itself
	    add 1 3 1	   but if we do end up adding one to the register, we add the current multiple of the multiplicand to the destination
skip    lw 0 4 mplier  now we need the multiplier back in place
		nor 5 5 5	   we also need '1' back in position
	    add 3 3 3      shift register 3, multiplicand, 1 place
	    add 5 5 5	   shift register 5 1 place
	    add 2 6 2	   subtract 1 from register 2
	    beq 0 0 start  jump back to start of loop
end	    noop
	    halt
iter	.fill 16
mcand   .fill 6203
mplier  .fill 1429
one 	.fill 1