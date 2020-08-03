#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H

#include "constants.h"

typedef enum {
    LT_COMMENT,
    LT_COMMAND,
    LT_DIRECTIVE
} LineType;

typedef enum {
    ST_CODE,
    ST_DATA,
    ST_EXTERNAL
} SymbolType;

typedef struct {
    char label[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    LineType lineType; /* TODO(yotam): Return line type -- done*/
} DissectedLine;

typedef struct {
    char command[MAX_CMD_LENGTH];
    char arg1[MAX_LINE_LENGTH];
    char arg2[MAX_LINE_LENGTH];
    int numArgs;
} CommandTokens;

enum AddressingType {
    AT_IMMEDIATE = 0,
    AT_DIRECT = 1,
    AT_RELATIVE = 2,
    AT_REGISTER = 3,
    AT_UNSET = -1
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
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int funct : 5;
    unsigned int destRegister : 3;
    unsigned int destAddressing : 2;
    unsigned int srcRegister : 3;
    unsigned int srcAddressing : 2;
    unsigned int opcode : 6;
} ByteCode;

typedef struct {
    int lineNumber;
    int IC;
    int DC;
    ByteCode currentByteCode[8192];
} State;




#endif /*ASSEMBLER_STRUCTS_H*/
