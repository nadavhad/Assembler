
label1: .data -1, 34,56,     89, 1234
label2: .string "GoodLabel"


; mov dsfsdf
mov #3, label1
mov #-23, r7

; just a comment lea sfsc

cmp label1, label2
cmp label2, r3

add #12000, label1
add label2, r4

sub #8, r5
sub r5, r6

lea label1, label2
lea label1, r2

clr STRING

not r2
not label2

inc r0
inc STRING

dec External
dec r5

C0: jmp label1
jmp &X


ENTRYLABEL: .string "abcd"
    .entry ENTRYLABEL

; blalalala 4%ds

DATA: bne X
bne &DATA

JUMP: jsr DATA
jsr &JUMP

red r1
red External

prn #2134
prn r7

rts

stop


STRING: .string "hello!"
X: .data 23
.extern External