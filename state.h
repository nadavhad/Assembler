#ifndef SHAREDFOLDER_STATE_H
#define SHAREDFOLDER_STATE_H

/**
 * A struct holding the state of the assembler
 */
typedef struct {
    int lineNumber;
    int IC;
    int DC;
    int ICF;
    int DCF;
    char currentByteCode[8192];
    char dataByteCode[4096];
} State;

/**
 * Initialize the global state container for an assembly file
 */
void initializeState();

/**
 * Get a ptr to the global state container
 * @return a ptr to the global state container
 */
State *getState();
/**
 * Get the current line number in the assembly file
 */
int getLineNumber();

/**
 * Increment the stored line number.
 * (Advance to the next line in the assembly file)
 */
void incLineNumber();

/**
 * Add a command and increment IC accordingly.
 */
void addCommand(const char *encodedOpcode, int opcodeLen);

/**
 * Add a data word and increment DC accordingly
 */
void addDataWord(long number);

#endif /*SHAREDFOLDER_STATE_H*/
