        lw      0       1       one     load 1 with 1
        lw      0       2       one     i = 1
        lw      0       4       inputN  load n into 4
        lw      0       5       one     out starts at 1
outer   nor     3       2       0       mov j i -> j = i | 0
        add     6       5       0       mov prevOut out
inner   add     5       5       6       add out to prevOut
        add     3       3       1       j += 1
        beq     2       3       inEnd   j == i -> done inner loop
        beq     0       0       inner   otherwise, go to top of inner
inEnd   add     2       2       1       i += 1
        beq     2       4       outEnd  i == out
        lw      0       7       otAddr  load jump address
        jalr    7       6               use jalr here for funny
outEnd  sw      0       5       output  store output
        halt                            halt
        noop
inputN  .fill   5
one     .fill   1
otAddr  .fill   outer                   will contain the address of outer
output  .fill   0
