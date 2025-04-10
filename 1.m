% processing function definition: 
evaluate     sw evaluatelink(r0),r15
     addi r1,r0,10
     sw t1(r0),r1
% processing: b := t1
     lw r1,t1(r0)
     sw b(r0),r1
% processing: return(evaluate)
     lw r1,b(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     jr r15
% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
% processing function definition: 
main     sw mainlink(r0),r15
     addi r1,r0,2
     sw t2(r0),r1
% processing: c := t2
     lw r1,t2(r0)
     sw c(r0),r1
% processing: function call to evaluate 
     jl r15,evaluate
     lw r1,evaluatereturn(r0)
     sw t3(r0),r1
% processing: c := t3
     lw r1,t3(r0)
     sw c(r0),r1
     addi r1,r0,2
     sw t4(r0),r1
     % processing: t5 := c + t4
     lw r2,c(r0)
     lw r3,t4(r0)
     add r1,r2,r3
     sw t5(r0),r1
% processing: put(t5)
     lw r1,t5(r0)
     sw -8(r14),r1
     addi r1,r0, buf
     sw -12(r14),r1
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     lw r15,mainlink(r0)
     hlt
% space for variable float
a      res 8
evaluatelink      res 4
evaluatereturn      res 4
% space for variable int
b      res 4
t1     res 4
% start of data code
% buffer space used for console output
mainlink      res 4
mainreturn      res 4
% space for variable int
c      res 4
% space for variable id
test      res 8
t2     res 4
     % space for function call expression factor
t3      res 4
t4     res 4
     % space for c + t4
t5      res 4
% buffer space used for console output
buf     res 20
