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

#endif /*ASSEMBLER_STRUCTS_H*/
