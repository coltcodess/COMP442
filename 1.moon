     entry
     addi     r14, r0, topaddr% Set stack pointer
 lw 
 lw 
 lw 
     lw     r2,a(r0)     % r2 := a
     lw     r3,b(r0)     % r3 := b
     mul    r1,r2,r3
     lw     r3,a(r0)     % r3 := a
     lw     r2,b(r0)     % r2 := b
     mul    r1,r3,r2
     lw     r2,a(r0)     % r2 := a
     lw     r3,b(r0)     % r3 := b
     mul    r1,r2,r3
     lw     r3,a(r0)     % r3 := a
     lw     r2,b(r0)     % r2 := b
     mul    r1,r3,r2
 lw 
     hlt
     % start of data code
     % buffer space used for console output
