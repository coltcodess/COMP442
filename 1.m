% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
     % processing function definition: 
evaluate     sw evaluatelink(r0),r15
     addi r1,r0,5
     sw t1(r0),r1
     % processing: return(t1)
     lw r1,t1(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     % processing function definition: 
main     sw mainlink(r0),r15
     % processing: function call to evaluate 
     jl r15,evaluate
     lw r1,evaluatereturn(r0)
     sw t2(r0),r1
     % processing: c := test
     lw r1,test(r0)
     sw c(r0),r1
     % processing: put(c)
     lw r1,c(r0)
     sw -8(r14),r1
     addi r1,r0, buf
     sw -12(r14),r1
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     lw r15,mainlink(r0)
     hlt
% start of data code
% buffer space used for console output
     % space for variable float
a      res 8
evaluatelink      res 4
evaluatereturn      res 4
t1     res 4
mainlink      res 4
mainreturn      res 4
     % space for variable int
c      res 4
     % space for variable id
test      res 8
     % space for function call expression factor
t2      res 4
     % buffer space used for console output
buf     res 20
