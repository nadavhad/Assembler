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

int findOperation(char *, Operation *);

int findCommandInTable(CommandTokens, Command);


#endif /*ASSEMBLER_PARSING_H*/
