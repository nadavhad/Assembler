#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H

#include "constants.h"

/**
 * An enum with the types of sloc (source lines of code) that can be:
 * empty/comment,
 * command,
 * directive
 */
typedef enum {
    LT_COMMENT,
    LT_COMMAND,
    LT_DIRECTIVE
} LineType;

/**
 * An enum of the diffferent argument addressing types:
 * Immediate (#number),
 * Direct (label),
 * Relative (&label),
 * Register (rnumber),
 * with UNSET for invalid
 */
enum AddressingType {
    AT_IMMEDIATE = 0,
    AT_DIRECT = 1,
    AT_RELATIVE = 2,
    AT_REGISTER = 3,
    AT_UNSET = -1
};

/**
 * An enum for symbol properties:
 * Code (commands),
 * Data (data/string directives),
 * External (external symbols)
 */
typedef enum {
    ST_CODE,
    ST_DATA,
    ST_EXTERNAL
} SymbolType;

/**
 * A struct containing data for a line of code,
 * dissected to the the label/symbol (empty if non-existant),
 * rest of line (command/directive + args),
 * and a LineType value indicating what type of line this is (see docs on the LineType enum)
 */
typedef struct {
    char label[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    LineType lineType;
} DissectedLine;

/**
 * An enum containing the different types of directives, and a invalid value
 */
enum DirectiveType {
    DT_UNDEFINED,
    DT_DATA,
    DT_STRING,
    DT_ENTRY,
    DT_EXTERN
};

/**
 * A container for relevant directive data: the type of directive, the raw code, and the argument string
 */
typedef struct {
    enum DirectiveType type;
    char directiveToken[10];
    char directiveArgs[MAX_LINE_LENGTH];
} DissectedDirective;

/**
 * Stores the literal value of an argument - a number or a symbol/label
 */
typedef union {
    long scalar;
    char symbol[MAX_LABEL_LENGTH];
} Data;

/**
 * A container for all argument-relevant data:
 * the addressing type,
 * argument position (src/dest),
 * the argument register,
 * and the argument value (number/symbol)
 */
typedef struct {
    enum AddressingType addressing;
    unsigned int reg;
    Data value;
} Argument;

/**
 * A container for all command data:
 * the command string itself,
 * argument strings and data containers for each argument
 */
typedef struct {
    char command[MAX_CMD_LENGTH];
    char arg1[MAX_LINE_LENGTH];
    Argument arg1Data;
    char arg2[MAX_LINE_LENGTH];
    Argument arg2Data;
    int numArgs;
} CommandTokens;

/**
 * A container for command/operation data:
 * opcode, funct, name, valid addressing types for both args, and the number of args
 */
typedef struct {
    int opcode;
    int funct;
    char name[5];
    int srcAddressing[5];
    int destAddressing[5];
    int numArgs;
} Operation;

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

#endif /*ASSEMBLER_STRUCTS_H*/
