        lw      0       1       12      load reg1 with 5 (symbolic address)
        jalr    1       4               jail load word into register 4, jump to address 5    
resume  nor     1       2       3       nor reg1 with reg 2 and put the result into reg3
        beq     0       0       1
        lw      1       2       9       load reg2 with -1 (numeric address)
jump    beq     0       0       -4      jump back
start   add     1       2       1       decrement reg1
        beq     0       1       1       goto end of program when reg1==0
        beq     0       0       -3      go back to the beginning of the loop
store   sw      1       3       20
        noop
done    halt                            end of program
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start                   will contain the address of start (2)