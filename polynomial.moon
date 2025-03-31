% start of program
     entry
     addi     r14, r0, topaddr % Set stack pointer
     addi r1,r02
     sw0(r14),r1
     addi r1,r02
     sw0(r14),r1
     addi r1,r02
     sw0(r14),r1
     addi r1,r02
     sw0(r14),r1
     addi r1,r00
     sw0(r14),r1
     sw     t1(r0),r1
     sw     t2(r0),r1
     sw     t4(r0),r1
     sw     t5(r0),r1
     sw     t7(r0),r1
     sw     t8(r0),r1
     sw     t10(r0),r1
     sw     t11(r0),r1
     sw     t13(r0),r1
     sw     t14(r0),r1
     sw     t16(r0),r1
     sw     t17(r0),r1
     addi r1,r01
     sw0(r14),r1
     addi r1,r01
     sw0(r14),r1
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
