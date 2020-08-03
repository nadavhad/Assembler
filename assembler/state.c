#include "structs.h"
#include <string.h>

State state;

State *getState(){
    return &state;
}


void initializeState() {
    getState()->lineNumber = 0;
    getState()->DC = 0;
    getState()->IC = 100;
    memset(getState()->currentByteCode, 0, sizeof(getState()->currentByteCode));
}

int getLineNumber() {
    return getState()->lineNumber;
}
void incLineNumber() {
    getState()->lineNumber++;
}


