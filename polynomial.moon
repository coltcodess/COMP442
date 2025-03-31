% start of program
     entry
     addi     r14, r0, topaddr% Set stack pointer
 lw 
     lw     r2,result(r0)     % r2 := result
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,result(r0)     % r3 := result
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
     lw     r2,result(r0)     % r2 := result
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,result(r0)     % r3 := result
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
 lw 
     lw     r2,result(r0)     % r2 := result
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,result(r0)     % r3 := result
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
     lw     r2,result(r0)     % r2 := result
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,result(r0)     % r3 := result
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
 lw 
 lw 
 lw 
 lw 
 lw 
 lw 
 lw 
     lw     r2,a(r0)     % r2 := a
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,a(r0)     % r3 := a
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
     lw     r2,a(r0)     % r2 := a
     lw     r3,x(r0)     % r3 := x
     mul    r1,r2,r3
     lw     r3,a(r0)     % r3 := a
     lw     r2,x(r0)     % r2 := x
     mul    r1,r3,r2
 lw 
 lw 
 lw 
 lw 
     hlt
% start of data code
% buffer space used for console output
% space for variable a
a           res 8
% space for variable b
b           res 8
% space for variable a
a           res 8
% space for variable b
b           res 8
% space for variable c
c           res 8
% space for variable result
result           res 8
% space for variable result
result           res 8
     % space for variable counter
counter           res 4
