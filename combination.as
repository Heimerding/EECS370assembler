        lw          0       1       n
        lw          0       2       r
        lw          0       6       Caddr       load combination function address
        jalr        6       7                   call function
        halt
start   lw          0       6       one         load 1 into reg6
saving  sw          5       7       Stack       save return address on Stack
        add         5       6       5           increment stack pointer
        sw          5       1       Stack       store n input
        add         5       6       5           increment stack pointer
        sw          5       2       Stack       store r input
        add         5       6       5           increment stack pointer
        sw          5       4       Stack       store local variable to stack
        add         5       6       5           increment stack pointer
        noop
        beq         0       2       ret1        If the register 2 (r-value) is equal to zero, jump straight to returning 1
        beq         1       2       ret1        Similarly, if the register 2 (r-value) is equal to n, jump straight to returning 1
        noop
self1   lw          0       6       neg1        load the temporary register with neg1
        add         1       6       1           decrement n
        lw          0       6       Caddr       load combination function address
        jalr        6       7                   call the function again with n-1
        add         0       3       4           put the value of register 3 (the return register) into register 4 - this should be the value of comb(n-1,r)
        noop
self2   lw          0       6       neg1        load the temporary register with neg1
        add         2       6       2           decrement r        
        lw          0       6       Caddr       load combination function address
        jalr        6       7                   call the function again with n-1 and r-1
        add         3       4       3           Add together the contents of temporary variable register 4 and return register 3, place in return register 3
        beq         0       0       rest        jump to the end of function with the return register correct
        noop
ret1    lw          0       3       one         fill the return register with 1
        noop
rest    lw          0       6       neg1        load -1 into register 6
        add         5       6       5           decrement stack pointer
        lw          5       4       Stack       reload the local variable 4 for the higher function
        add         5       6       5           decrement stack pointer
        lw          5       2       Stack       recover original r-input
        add         5       6       5           decrement stack pointer
        lw          5       1       Stack       recover original n-input
        add         5       6       5           decrement stack pointer
        lw          5       7       Stack       recover original return pointer
ret     jalr        7       6                   return to function call
n       .fill       3
r       .fill       1
Caddr   .fill       start
one     .fill       1
neg1    .fill       -1
Stack   .fill       0
