; This file should make erorrs in the first pass

; label errors
LABEL1: .data -2,940,+24,-980
LABEL2: mov r0,r6
ANOTHERONE: add r3,r7
    .extern 12LABEL
STRING: .string "just a string
LABEL1: .string "already defined this label"
LABEL2: .data "same problem here"
    .extern ANOTHERONE
    .extern STRING
    .extern DKFSDKFASDJLFJLKJFKLSDJFLJFLKJKJLKJJ
SHOULDF&IL: data 345,67

; data and string errors
    .string "still open
    .string not opened"
    .string no quotation marks
    .data ,3, 15
    .data 2,234,23,,6
    .data 1,89,
    .data 1 3 , 23
    .data 9.9

; Wrong number of arguments
mov
mov r6
add
sub LABEL
lea operand
clr
not
dec
jmp
bne &realtivelyjump, #12
jsr r1, no
red one,three
prn #-13, more
rts bla
stop something

cmp #-0,A,r3
add #-4,#-9,#7
inc r-9,r1,r4


; Addressing type errors
mov &arg1, &arg2
cmp &arg1, arg2
add #23,#36
sub arg1, #100
lea #-1, #99

clr &arg1
not &arg1
inc #-31
dec &arg1
jmp arg1
bne arg1
jsr #345

red &arg1
prn &arg2

; Undefined operations
ad r1
nrg #36, #12




