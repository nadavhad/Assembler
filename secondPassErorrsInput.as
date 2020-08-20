; This file should make some erorrs in the second pass

DataIsNice: .data -1,5,-99,1024


; wont find labels
    .entry LABEL
    .entry MOREERRORS
    .entry DATA12
    .entry SUM

; Using labels that dont exist
prn ALABEL
prn MAN
dec ERORR
inc B
dec GO
jsr NOTDEF

; wont find labels
    .entry ARE
    .entry FOR
    .entry HELLO
    .entry ONE






