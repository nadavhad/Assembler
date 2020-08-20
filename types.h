#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

/**
 * A boolean data type
 */
enum bool {
    FALSE, TRUE
};

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

#endif /*ASSEMBLER_TYPES_H*/
