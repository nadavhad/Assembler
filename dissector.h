#include "structs.h"

#ifndef ASSEMBLER_DISSECTOR_H
#define ASSEMBLER_DISSECTOR_H

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
 * A container for relevant directive data: the type of directive, the raw code, and the argument string
 */
typedef struct {
    enum DirectiveType type;
    char directiveToken[10];
    char directiveArgs[MAX_LINE_LENGTH];
} DissectedDirective;
/**
 * Splits a line of assembly code into a struct containing the label and command (with its args)
 * and also identifies the coarse line type.
 * rawLine - Input line
 * dissectedLine - the dissected input line
 * returns 0 on success.
 */
int dissectLabel(char *, DissectedLine *);

/**
 * Splits a command string into a command and its arguments
 * @return
 */
int dissectCommand(char *line, CommandTokens *);

/**
 * Finds what operation it is, fills the Operation struct with relevant data.
 * @return 0 on success, -1 on failure
 */
int findOperation(char *, Operation *);

/**
 * Finds the addressing type of the argument, and checks its validity.
 * @return addressing type (0..3), or -1 on error/mismatch
 */
int findArgumentAddressingType(const char *, Argument *);

/**
 * Validate that the argument is a valid label (no reserved words, alphanumric characters, etc.)
 * @return 0 is valid, 1 otherwise.
 */
int validateLabel(const char *label);

int getDirectiveType(DissectedLine dissectedLine, DissectedDirective *directive);

#define ENTRY ".entry"
#define DATA ".data"
#define EXTERN ".extern"
#define STRING ".string"

#endif /*ASSEMBLER_PARSING_H*/
