     addi r1,r0,2
     sw t1(r0),r1
     addi r1,r0,2
     sw t2(r0),r1
% processing function definition: 
evaluate     sw evaluatelink(r0),r15
     addi r1,r0,0
     sw t3(r0),r1
% processing: return(evaluate)
     lw r1,t3(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     jr r15
% processing function definition: 
evaluate     sw evaluatelink(r0),r15
% processing: result := a
     lw r1,a(r0)
     sw result(r0),r1
% processing: t4 := result + x
     lw r2,result(r0)
     lw r3,x(r0)
     mul r1,r2,r3
     sw t4(r0),r1
% processing: t5 := t4 + b
     lw r2,t4(r0)
     lw r3,b(r0)
     add r1,r2,r3
     sw t5(r0),r1
% processing: result := t5
     lw r1,t5(r0)
     sw result(r0),r1
% processing: t6 := result + x
     lw r2,result(r0)
     lw r3,x(r0)
     mul r1,r2,r3
     sw t6(r0),r1
% processing: t7 := t6 + c
     lw r2,t6(r0)
     lw r3,c(r0)
     add r1,r2,r3
     sw t7(r0),r1
% processing: result := t7
     lw r1,t7(r0)
     sw result(r0),r1
% processing: return(evaluate)
     lw r1,result(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     jr r15
% processing function definition: 
build     sw buildlink(r0),r15
% processing: new_function := A
     lw r1,A(r0)
     sw new_function(r0),r1
% processing: new_function := B
     lw r1,B(r0)
     sw new_function(r0),r1
% processing: new_function := C
     lw r1,C(r0)
     sw new_function(r0),r1
% processing: return(build)
     lw r1,new_function(r0)
     sw buildreturn(r0),r1
     lw r15,buildlink(r0)
     jr r15
% processing function definition: 
build     sw buildlink(r0),r15
% processing: new_function := A
     lw r1,A(r0)
     sw new_function(r0),r1
% processing: new_function := B
     lw r1,B(r0)
     sw new_function(r0),r1
% processing: return(build)
     lw r1,new_function(r0)
     sw buildreturn(r0),r1
     lw r15,buildlink(r0)
     jr r15
% processing function definition: 
evaluate     sw evaluatelink(r0),r15
% processing: result := 
     lw r1,(r0)
     sw result(r0),r1
% processing: t8 := a + x
     lw r2,a(r0)
     lw r3,x(r0)
     mul r1,r2,r3
     sw t8(r0),r1
% processing: t9 := t8 + b
     lw r2,t8(r0)
     lw r3,b(r0)
     add r1,r2,r3
     sw t9(r0),r1
% processing: result := t9
     lw r1,t9(r0)
     sw result(r0),r1
% processing: return(evaluate)
     lw r1,result(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     jr r15
% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
% processing function definition: 
main     sw mainlink(r0),r15
     addi r1,r0,2
     sw t10(r0),r1
% processing: function call to build 
     jl r15,build
     lw r1,buildreturn(r0)
     sw t11(r0),r1
% processing: f1 := t11
     lw r1,t11(r0)
     sw f1(r0),r1
% processing: function call to build 
     jl r15,build
     lw r1,buildreturn(r0)
     sw t12(r0),r1
% processing: f2 := t12
     lw r1,t12(r0)
     sw f2(r0),r1
     addi r1,r0,1
     sw t13(r0),r1
% processing: counter := t13
     lw r1,t13(r0)
     sw counter(r0),r1
% processing: WHILE statement 
gowhile1     
     addi r2,r0,10
     sw t14(r0),r2
% processing: relop counter  10
     lw r2,counter(r0)
     lw r3,t14(r0)
      r4,r2,r3
     sw t15(r0),r4
     lw r1,t15(r0)
     bz r1,endwhile1
% processing: put(counter)
     lw r2,counter(r0)
     sw -8(r14),r2
     addi r2,r0, buf
     sw -12(r14),r2
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
% processing: function call to evaluate 
     jl r15,evaluate
     lw r2,evaluatereturn(r0)
     sw t16(r0),r2
% processing: put(t16)
     lw r2,t16(r0)
     sw -8(r14),r2
     addi r2,r0, buf
     sw -12(r14),r2
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
% processing: function call to evaluate 
     jl r15,evaluate
     lw r2,evaluatereturn(r0)
     sw t17(r0),r2
% processing: put(t17)
     lw r2,t17(r0)
     sw -8(r14),r2
     addi r2,r0, buf
     sw -12(r14),r2
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
     j gowhile1
endwhile1     
     lw r15,mainlink(r0)
     hlt
% space for variable float
a      res 8
% space for variable float
b      res 8
t1     res 4
t2     res 4
% space for variable float
a      res 8
% space for variable float
b      res 8
% space for variable float
c      res 8
evaluatelink      res 4
evaluatereturn      res 4
t3     res 4
evaluatelink      res 4
evaluatereturn      res 4
% space for variable float
result      res 8
     % space for result + x
t4      res 4
     % space for t4 + b
t5      res 4
     % space for result + x
t6      res 4
     % space for t6 + c
t7      res 4
buildlink      res 4
buildreturn      res 4
% space for variable id
new_function      res 72
buildlink      res 4
buildreturn      res 4
% space for variable id
new_function      res 16
evaluatelink      res 4
evaluatereturn      res 4
% space for variable float
result      res 8
     % space for a + x
t8      res 4
     % space for t8 + b
t9      res 4
% start of data code
% buffer space used for console output
mainlink      res 4
mainreturn      res 4
% space for variable id
f1      res 16
% space for variable id
f2      res 72
% space for variable int
counter      res 4
t10     res 4
     % space for function call expression factor
t11      res 4
     % space for function call expression factor
t12      res 4
t13     res 4
t14     res 4
t15     res 4
     % space for function call expression factor
t16      res 4
     % space for function call expression factor
t17      res 4
% buffer space used for console output
buf     res 20
