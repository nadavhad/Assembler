#include "structs.h"

State state;

State *getState(){
    return &state;
}


void initializeState() {
    getState()->lineNumber = 0;
    getState()->DC = 0;
    getState()->IC = 100;
}

int getLineNumber() {
    return getState()->lineNumber;
}
void incLineNumber() {
    getState()->lineNumber++;
}


