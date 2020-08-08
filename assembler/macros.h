

#ifndef ASSEMBLER_MACROS_H
#define ASSEMBLER_MACROS_H

#define NUM_OPERATIONS 16
#define EQ(c, n) ((c) == (n))
#define EOS(c) EQ(c,'\0')
#define eqEOF(c) EQ(c, EOF)
#define EOL(c) (EQ(c,'\n') || EQ(c, '\r'))

#define END(c) (EOS(c) || eqEOF(c) || EOL(c))
#define WHT(c) (EQ(c, ' ') || EQ(c, '\t'))


#endif /*ASSEMBLER_MACROS_H*/
