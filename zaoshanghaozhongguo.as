        lw      0       2       n       arg1 = r2 = n
        lw      0       6       fibSt   r6 = jump target = &fib
        jalr    6       7               jump to fib
        sw      0       1       res     on return store result from r1
        halt                            
fib     lw      0       6       neg1    r6 = -1
        lw      0       4       one     r4 = f(n - 1) = 1
top     add     1       4       5       r5 = f(n - 1) + f(n - 2) = r1 + r4
        add     0       1       4       f(n - 1) = r4 = r1
        add     0       5       1       f(n) = r1 = r5
        add     2       6       2       r2 -= 1
        beq     2       0       end
        beq     0       0       top
end     jalr    7       6
n       .fill   10
res     .fill   0
one     .fill   1
neg1    .fill   -1
fibSt   .fill   fib