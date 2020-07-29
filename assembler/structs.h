#include "constants.h"

#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H

enum LineType {
    LT_COMMENT,
    LT_COMMAND,
    LT_DIRECTIVE
};
typedef struct {
    char label[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    enum LineType lineType; /* TODO(yotam): Return line type -- done*/
} DissectedLine;

typedef struct {
    char command[MAX_CMD_LENGTH];
    char arg1[MAX_LINE_LENGTH];
    char arg2[MAX_LINE_LENGTH];
    int numArgs;
} CommandTokens;

typedef struct {
    int deleteThis;
    /*TODO(nadav): Define contents*/
} Command;

enum AddressingType {
    AT_IMMEDIATE, AT_DIRECT, AT_RELATIVE, AT_REGISTER
};

typedef struct {
    int opcode;
    int funct;
    char name[5];
    int srcAddressing[5];
    int destAddressing[5];
} Operation;

#endif /*ASSEMBLER_STRUCTS_H*/
