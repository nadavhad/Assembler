#ifndef SHAREDFOLDER_STATE_H
#define SHAREDFOLDER_STATE_H

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
 * @return
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
#endif /*SHAREDFOLDER_STATE_H*/
