#ifndef SHAREDFOLDER_STATE_H
#define SHAREDFOLDER_STATE_H

typedef struct {
    int lineNumber;
    int IC;
    int DC;
    char currentByteCode[8192];
    char dataByteCode[4096];
} State;

void initializeState();
State *getState();
int getLineNumber();
void incLineNumber();
#endif /*SHAREDFOLDER_STATE_H*/
