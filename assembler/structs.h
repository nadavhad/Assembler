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
    AT_IMMEDIATE = 0, AT_DIRECT = 1, AT_RELATIVE = 2, AT_REGISTER = 3, AT_UNSET=-1
};

typedef struct {
    int opcode;
    int funct;
    char name[5];
    int srcAddressing[5];
    int destAddressing[5];
    int numArgs;
} Operation;

typedef struct {
    int lineNumber;
    int IC;
    int DC;
} State;

#endif /*ASSEMBLER_STRUCTS_H*/
