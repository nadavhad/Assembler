#include <stdio.h>
#include "constants.h"
#ifndef ASSEMBLER_PARSING_H
#define ASSEMBLER_PARSING_H

/* A shorthand macro useful for comparing two chars, especially nested in other macros */
#define EQ(c, n) ((c) == (n))
/* Checks if a certain character is NUL ('\0') */
#define EOS(c) EQ(c,'\0')
/* Checks if a certain character is EOF */
#define eqEOF(c) EQ(c, EOF)
/* Checks if a certain character is the end of a line: newline or carriage return */
#define EOL(c) (EQ(c,'\n') || EQ(c, '\r'))
/* Checks if a certain character is NUL ('\0'), EOF, newline or carriage return */
#define END(c) (EOS(c) || eqEOF(c) || EOL(c))
/* Checks if a certain character is whitespace, excluding newline */
#define WHT(c) (EQ(c, ' ') || EQ(c, '\t') || EOL(c))

/**
 * Copy the input string (rawStr) without leading and trailing whitespaces into stripped.
 */
int stripWhiteSpaces(char *rawStr, char stripped[MAX_LINE_LENGTH]);


#endif /*ASSEMBLER_PARSING_H*/
