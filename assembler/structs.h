#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H

#include "constants.h"

typedef enum {
    LT_COMMENT,
    LT_COMMAND,
    LT_DIRECTIVE
} LineType;

enum AddressingType {
    AT_IMMEDIATE = 0,
    AT_DIRECT = 1,
    AT_RELATIVE = 2,
    AT_REGISTER = 3,
    AT_UNSET = -1
};

typedef enum {
    ST_CODE,
    ST_DATA,
    ST_EXTERNAL
} SymbolType;

typedef struct {
    char label[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    LineType lineType;
} DissectedLine;


enum DirectiveType {
    DT_UNDEFINED,
    DT_DATA,
    DT_STRING,
    DT_ENTRY,
    DT_EXTERN
};

typedef struct {
    enum DirectiveType type;
    char directiveToken[10];
    char directiveArgs[MAX_LINE_LENGTH];
} DissectedDirective;

enum ArgumentPosition {
    AP_SRC = 1, AP_DEST = 2
};
typedef union {
    long scalar;
    char symbol[MAX_LABEL_LENGTH];
} Data;

typedef struct {
    enum AddressingType addressing;
    int position;
    unsigned int reg;
    Data value;
} Argument;

typedef struct {
    char command[MAX_CMD_LENGTH];
    char arg1[MAX_LINE_LENGTH];
    Argument arg1Data;
    char arg2[MAX_LINE_LENGTH];
    Argument arg2Data;
    int numArgs;
} CommandTokens;

typedef struct {
    int opcode;
    int funct;
    char name[5];
    int srcAddressing[5];
    int destAddressing[5];
    int numArgs;
} Operation;

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


typedef struct {
    unsigned int E: 1;
    unsigned int R: 1;
    unsigned int A: 1;
    unsigned int data: 21;
} EncodedArg;

#endif /*ASSEMBLER_STRUCTS_H*/
