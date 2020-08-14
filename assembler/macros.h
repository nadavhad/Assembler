#ifndef ASSEMBLER_MACROS_H
#define ASSEMBLER_MACROS_H
/* Number of Operations available in .as syntax*/
#define NUM_OPERATIONS 16
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
#define WHT(c) (EQ(c, ' ') || EQ(c, '\t'))


#endif /*ASSEMBLER_MACROS_H*/
