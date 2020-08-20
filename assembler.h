#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include "dissector.h"

int handleDirective(DissectedDirective dissectedDirective);

/**
 * Handle command code all the way to building and saving machine code
 */
int handleCommand(DissectedLine dissectedLine);

/**
 * The bit-encoded command/operation data/"word"
 */
typedef struct {
    unsigned int E: 1;
    unsigned int R: 1;
    unsigned int A: 1;
    unsigned int funct: 5;
    unsigned int destRegister: 3;
    unsigned int destAddressing: 2;
    unsigned int srcRegister: 3;
    unsigned int srcAddressing: 2;
    unsigned int opcode: 6;
} EncodedOperation;

/**
 * The bit-encoded data for argument "words"
 */
typedef struct {
    unsigned int E: 1;
    unsigned int R: 1;
    unsigned int A: 1;
    unsigned int data: 21;
} EncodedArg;

#endif /*ASSEMBLER_ASSEMBLER_H*/
