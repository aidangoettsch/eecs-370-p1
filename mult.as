        lw      0       2       mcand   r2 = mcand
        lw      0       3       mplier  r3 = mplier
        lw      0       4       one     r4 = scan bit
        lw      0       7       term    r7 = term = 2**n
top     nor     0       4       4       r4 = ~r4
        nor     0       3       3       r3 = ~r3
        nor     3       4       5       r5 = ~(~mplier + ~scan bit) = mplier & scan bit
        beq     5       0       skip    r5 == 0 -> skip adding mcand to out
        add     2       1       1       out += mcand
skip    nor     0       4       4       reset r4 to be scan bit
        nor     0       3       3       reset r3 to be mplier
        add     2       2       2       mcand << 1
        add     4       4       4       scan bit << 1
        beq     7       4       end     scan bit == 2**n
        beq     0       0       top     go back to top
end    halt
one     .fill   1
mcand   .fill   6203
mplier  .fill   1429
term    .fill   32768