% processing function definition: 
evaluate     sw evaluatelink(r0),r15
     addi r1,r0,0
     sw t1(r0),r1
% processing: return(evaluate)
     lw r1,t1(r0)
     sw evaluatereturn(r0),r1
     lw r15,evaluatelink(r0)
     jr r15
% processing function definition: 
build     sw buildlink(r0),r15
% processing: new_functiontion := A
     lw r1,A(r0)
     sw new_functiontion(r0),r1
% processing: new_functiontion := B
     lw r1,B(r0)
     sw new_functiontion(r0),r1
% processing: return(build)
     lw r1,new_functiontion(r0)
     sw buildreturn(r0),r1
     lw r15,buildlink(r0)
     jr r15
% processing function definition: 
evaluate     sw evaluatelink(r0),r15
% processing: result := 
     lw r1,(r0)
     sw result(r0),r1
% processing: t2 := a + x
     lw r2,a(r0)
     lw r3,x(r0)
     mul r1,r2,r3
     sw t2(r0),r1
% processing: t3 := t2 + b
     lw r2,t2(r0)
     lw r3,b(r0)
     add r1,r2,r3
     sw t3(r0),r1
% processing: result := t3
     lw r1,t3(r0)
     sw result(r0),r1
% processing: return(evaluate)
     lw r1,result(r0)
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
     addi r1,r0,3
     sw t8(r0),r1
% processing: t9 := B + t8
     lw r2,B(r0)
     lw r3,t8(r0)
     mul r1,r2,r3
     sw t9(r0),r1
% processing: t10 := A + t9
     lw r2,A(r0)
     lw r3,t9(r0)
     add r1,r2,r3
     sw t10(r0),r1
% processing: new_functiontion := t10
     lw r1,t10(r0)
     sw new_functiontion(r0),r1
% processing: new_functiontion := B
     lw r1,B(r0)
     sw new_functiontion(r0),r1
     addi r1,r0,1
     sw t11(r0),r1
% processing: new_functiontion := t11
     lw r1,t11(r0)
     sw new_functiontion(r0),r1
     addi r1,r0,1
     sw t12(r0),r1
% processing: return(build)
     lw r1,t12(r0)
     sw buildreturn(r0),r1
     lw r15,buildlink(r0)
     jr r15
% processing function definition: 
build2     sw build2link(r0),r15
% processing: new_functiontion := A
     lw r1,A(r0)
     sw new_functiontion(r0),r1
% processing: new_functiontion := B
     lw r1,B(r0)
     sw new_functiontion(r0),r1
% processing: new_functiontion := C
     lw r1,C(r0)
     sw new_functiontion(r0),r1
% processing: return(build2)
     lw r1,new_functiontion(r0)
     sw build2return(r0),r1
     lw r15,build2link(r0)
     jr r15
% start of program
     entry
     addi r14, r0, topaddr % Set stack pointer
% processing function definition: 
f     sw flink(r0),r15
     lw r15,flink(r0)
% processing function definition: 
f     sw flink(r0),r15
     lw r15,flink(r0)
% processing function definition: 
f     sw flink(r0),r15
     lw r15,flink(r0)
% processing function definition: 
f3     sw f3link(r0),r15
     addi r1,r0,2
     sw t13(r0),r1
     addi r1,r0,3
     sw t14(r0),r1
     lw r15,f3link(r0)
% processing function definition: 
main     sw mainlink(r0),r15
     addi r1,r0,2
     sw t15(r0),r1
     addi r1,r0,3
     sw t16(r0),r1
     addi r1,r0,1
     sw t17(r0),r1
     addi r1,r0,2
     sw t18(r0),r1
     addi r1,r0,3
     sw t19(r0),r1
     addi r1,r0,2
     sw t20(r0),r1
% processing: function call to build 
     jl r15,build
     lw r1,buildreturn(r0)
     sw t21(r0),r1
% processing: f1 := t21
     lw r1,t21(r0)
     sw f1(r0),r1
% processing: function call to build 
     jl r15,build
     lw r1,buildreturn(r0)
     sw t22(r0),r1
% processing: f2 := t22
     lw r1,t22(r0)
     sw f2(r0),r1
     addi r1,r0,1
     sw t23(r0),r1
% processing: c := t23
     lw r1,t23(r0)
     sw c(r0),r1
% processing: function call to f 
     jl r15,f
     lw r1,freturn(r0)
     sw t24(r0),r1
% processing: counter := t24
     lw r1,t24(r0)
     sw counter(r0),r1
% processing: counter := a
     lw r1,a(r0)
     sw counter(r0),r1
     addi r1,r0,1
     sw t25(r0),r1
     addi r1,r0,1
     sw t26(r0),r1
     addi r1,r0,1
     sw t27(r0),r1
     addi r1,r0,1
     sw t28(r0),r1
     addi r1,r0,2
     sw t29(r0),r1
     addi r1,r0,1
     sw t30(r0),r1
% processing: i := t30
     lw r1,t30(r0)
     sw i(r0),r1
     addi r1,r0,2
     sw t31(r0),r1
     addi r1,r0,2
     sw t32(r0),r1
% processing: i := t32
     lw r1,t32(r0)
     sw i(r0),r1
     addi r1,r0,1
     sw t33(r0),r1
% processing: counter := t33
     lw r1,t33(r0)
     sw counter(r0),r1
% processing: WHILE statement 
gowhile1     
     addi r2,r0,10
     sw t34(r0),r2
% processing: relop counter  10
     lw r2,counter(r0)
     lw r3,t34(r0)
      r4,r2,r3
     sw t35(r0),r4
     lw r1,t35(r0)
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
     sw t36(r0),r2
% processing: put(t36)
     lw r2,t36(r0)
     sw -8(r14),r2
     addi r2,r0, buf
     sw -12(r14),r2
     jl r15, intstr
     sw -8(r14),r13
     jl r15, putstr
% processing: function call to evaluate 
     jl r15,evaluate
     lw r2,evaluatereturn(r0)
     sw t37(r0),r2
% processing: put(t37)
     lw r2,t37(r0)
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
% space for variable int
b      res 4
% space for variable float
a      res 8
% space for variable float
b      res 8
% space for variable float
c      res 8
% space for variable int
a      res 4
% space for variable int
b      res 4
% space for variable int
c      res 4
evaluatelink      res 4
evaluatereturn      res 4
t1     res 4
buildlink      res 4
buildreturn      res 4
% space for variable id
new_functiontion      res 20
evaluatelink      res 4
evaluatereturn      res 4
% space for variable float
result      res 8
     % space for a + x
t2      res 4
     % space for t2 + b
t3      res 4
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
new_functiontion      res 44
t8     res 4
     % space for B + t8
t9      res 4
     % space for A + t9
t10      res 4
t11     res 4
t12     res 4
build2link      res 4
build2return      res 4
% space for variable id
new_functiontion      res 44
% start of data code
% buffer space used for console output
flink      res 4
freturn      res 4
flink      res 4
freturn      res 4
flink      res 4
freturn      res 4
f3link      res 4
f3return      res 4
t13     res 4
t14     res 4
mainlink      res 4
mainreturn      res 4
% space for variable id
a      res 0
% space for variable id
c      res 48
% space for variable id
f1      res 20
% space for variable id
f2      res 44
% space for variable int
counter      res 4
% space for variable float
counter      res 8
t15     res 4
t16     res 4
% space for variable int
i      res 4
t17     res 4
t18     res 4
t19     res 4
% space for variable int
j      res 4
t20     res 4
     % space for function call expression factor
t21      res 4
     % space for function call expression factor
t22      res 4
t23     res 4
     % space for function call expression factor
t24      res 4
t25     res 4
t26     res 4
t27     res 4
t28     res 4
t29     res 4
t30     res 4
t31     res 4
t32     res 4
t33     res 4
t34     res 4
t35     res 4
     % space for function call expression factor
t36      res 4
     % space for function call expression factor
t37      res 4
% buffer space used for console output
buf     res 20
