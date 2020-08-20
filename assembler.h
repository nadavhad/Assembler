#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include "dissector.h"

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

int handleDirective(DissectedDirective dissectedDirective);

int handleDirectiveLabelFirstPass(DissectedLine line, DissectedDirective dissectedDirective);

int handleCommand(DissectedLine dissectedLine);

int verifyArguments(Operation *op, CommandTokens *commandTokens);

/**
 *
 * @param validAddressingArr
 * @param arg
 * @param argData
 * @return
 */
int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData);

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
