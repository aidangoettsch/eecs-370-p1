        lw      0       1       n
        lw      0       2       r
        lw      0       4       Caddr   load combination function address
        jalr    4       7               call function
        halt
nCr     lw      0       6       zero
        beq     2       6       base    r == 0 -> base
        beq     1       2       base    n == r -> base
        noop
        sw      5       7       Stack   push return address to stack
        lw      0       6       one     add one to stack pointer
        add     5       6       5       ^^^^^
        lw      0       6       negone  sub one from n
        add     1       6       1       ^^^^^
        noop
        sw      5       1       Stack   push n - 1 to stack
        lw      0       6       one     add one to stack pointer
        add     5       6       5       ^^^^^
        sw      5       2       Stack   push r to stack
        lw      0       6       one     add one to stack pointer
        add     5       6       5       ^^^^^
        noop
        lw      0       6       Caddr   call nCr(n - 1, r)
        jalr    6       7               ^^^^^
        add     0       3       4       r4 = return from nCr(n - 1, r)
        noop
        lw      0       6       negone  sub one from stack pointer
        add     5       6       5       ^^^^^
        lw      5       2       Stack   pop r from stack
        lw      0       6       negone  sub one from stack pointer
        add     5       6       5       ^^^^^
        lw      5       1       Stack   pop n - 1 from stack
        sw      5       4       Stack   push r4 to stack
        lw      0       6       one     add one to stack pointer
        add     5       6       5       ^^^^^
        lw      0       6       negone  sub one from r
        add     2       6       2       ^^^^^
        noop
        lw      0       6       Caddr   call nCr(n - 1, r - 1)
        jalr    6       7               ^^^^^
        noop
        lw      0       6       negone  sub one from stack pointer
        add     5       6       5       ^^^^^
        lw      5       4       Stack   pop r4 from stack
        add     3       4       3       r3 += r4
        lw      0       6       negone  sub one from stack pointer
        add     5       6       5       ^^^^^
        lw      5       7       Stack   pop return address from stack
        jalr    7       6               return
        noop
base    lw      0       3       one    
        jalr    7       4
n       .fill   7
r       .fill   3
Caddr   .fill   nCr
zero    .fill   0
negone  .fill   -1
one     .fill   1
Stack   .fill   0
