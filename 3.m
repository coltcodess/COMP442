% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
% processing function definition: 
main     sw mainlink(r0),r15
     addi r1,r0,0
     sw t1(r0),r1
% processing: c := t1
     lw r1,t1(r0)
     sw c(r0),r1
% processing: WHILE statement 
gowhile1     
     addi r2,r0,5
     sw t2(r0),r2
% processing: relop c clt 5
     lw r2,c(r0)
     lw r3,t2(r0)
     clt r4,r2,r3
     sw t3(r0),r4
     lw r1,t3(r0)
     bz r1,endwhile1
% processing: put(c)
     lw r2,c(r0)
     sw -8(r14),r2
     addi r2,r0, buf
     sw -12(r14),r2
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     addi r2,r0,1
     sw t4(r0),r2
% processing: t5 := c + t4
     lw r3,c(r0)
     lw r4,t4(r0)
     add r2,r3,r4
     sw t5(r0),r2
% processing: c := t5
     lw r2,t5(r0)
     sw c(r0),r2
     j gowhile1
endwhile1     
     lw r15,mainlink(r0)
     hlt
% start of data code
% buffer space used for console output
mainlink      res 4
mainreturn      res 4
% space for variable int
c      res 4
t1     res 4
t2     res 4
t3     res 4
t4     res 4
     % space for c + t4
t5      res 4
% buffer space used for console output
buf     res 20
