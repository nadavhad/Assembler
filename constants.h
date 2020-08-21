#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

/* Maximum length of a line from the input */
#define MAX_LINE_LENGTH 82
/* Maximum command length (actually < 4) */
#define MAX_CMD_LENGTH 5
/* Maximum length of a label (in characters) */
#define MAX_LABEL_LENGTH 31
/* Maximum amount of parameters in any function */
#define MAX_PARAMS 2
/* Maximum amount of tokens from any line*/
#define MAX_TOKENS 4
/* Maximum length of a file name */
#define MAX_FILE_NAME 100
/*Maximum positive number that can fit into of 24 bits (the size of a "word")*/
#define MAX_24_BIT_WORD 8388607
/*Minimum negative number that can fit into of 24 bits (the size of a "word")*/
#define MIN_24_BIT_WORD -8388608
/*Maximum positive number that can fit into of 21 bits (the size of a "word" excluding A R E bits)*/
#define MAX_21_BIT_WORD 1048575
/*Minimum negative number that can fit into of 21 bits (the size of a "word" excluding A R E bits)*/
#define MIN_21_BIT_WORD -1048576

/* Number of Operations available in .as syntax*/
#define NUM_OPERATIONS 16

#endif /*ASSEMBLER_CONSTANTS_H*/
