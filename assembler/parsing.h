#include "structs.h"

#ifndef ASSEMBLER_PARSING_H
#define ASSEMBLER_PARSING_H


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
int dissectCommand(char *line, int lineNumber, CommandTokens *);

/**
 * Finds what operation it is, fills the Operation struct with relevant data.
 * @return 0 on success, -1 on failure
 */
int findOperation(char *, Operation *);

/**
 * Possible duplicate of findOperation(char*, Operation*)
 * @return
 */
int findCommandInTable(CommandTokens, Command);


#endif /*ASSEMBLER_PARSING_H*/
