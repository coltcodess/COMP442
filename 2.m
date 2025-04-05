% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
     addi r1,r0,10
     sw t1(r0),r1
     % processing: b := t1
     lw r1,t1(r0)
     sw b(r0),r1
     addi r1,r0,5
     sw t2(r0),r1
     % processing: c := t2
     lw r1,t2(r0)
     sw c(r0),r1
     % processing: t3 := c + b
     lw r2,c(r0)
     lw r3,b(r0)
     add r1,r2,r3
     sw t3(r0),r1
     % processing: put(t3)
     lw r1,t3(r0)
     sw -8(r14),r1
     addi r1,r0, buf
     sw -12(r14),r1
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     hlt
% start of data code
% buffer space used for console output
     % space for variable int
c      res 4
     % space for variable int
b      res 4
t1     res 4
t2     res 4
     % space for c + b
t3      res 4
     % buffer space used for console output
buf     res 20
