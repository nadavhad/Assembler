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

enum AddressingType {
    AT_IMMEDIATE, AT_DIRECT, AT_RELATIVE, AT_REGISTER
};

typedef struct {
    int opcode;
    int funct;
    char name[5];
    int srcAddressingTypes[5];
    int destAddressingTypes[5];
} Operation;

typedef struct {
    int lineNumber;
    int IC;
    int DC;
} State;

#endif /*ASSEMBLER_STRUCTS_H*/