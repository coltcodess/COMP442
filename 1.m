% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
     addi r1,r0,2
     sw t1(r0),r1
     addi r1,r0,10
     sw t2(r0),r1
     % processing: b := t2
     lw r1,t2(r0)
     sw b(r0),r1
     addi r1,r0,42
     sw t3(r0),r1
     % processing: a := t3
     lw r1,t3(r0)
     sw a(r0),r1
     % processing: t4 := b + a
     lw r2,b(r0)
     lw r3,a(r0)
     add r1,r2,r3
     sw t4(r0),r1
     % processing: c := t4
     lw r1,t4(r0)
     sw c(r0),r1
     % processing: put(c)
     lw r1,c(r0)
     sw -8(r14),r1
     addi r1,r0, buf
     sw -12(r14),r1
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     hlt
% start of data code
% buffer space used for console output
t1     res 4
     % space for variable int
a      res 4
     % space for variable int
b      res 4
     % space for variable int
c      res 4
t2     res 4
t3     res 4
     % space for b + a
t4      res 4
     % buffer space used for console output
buf     res 20
