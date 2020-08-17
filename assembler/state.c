#include "structs.h"
#include "../logging/errorlog.h"
#include <string.h>

State state;

State *getState() {
    return &state;
}


void initializeState() {
    getState()->lineNumber = 0;
    getState()->DC = 0;
    getState()->IC = 100;
    getState()->DCF = -1;
    getState()->ICF = -1;
    memset(getState()->currentByteCode, 0, sizeof(getState()->currentByteCode));
}

int getLineNumber() {
    return getState()->lineNumber;
}

void incLineNumber() {
    getState()->lineNumber++;
}

void addCommand(const char *encodedOpcode, int opcodeLen) {
    memcpy(&(getState()->currentByteCode[getState()->IC*3]), encodedOpcode, opcodeLen*3);
    getState()->IC += opcodeLen;
}

void addDataWord(long number) {
    memset(&(getState()->dataByteCode[3 * getState()->DC]), number < 0 ? 0xFF : 0, 3);
    *((long*)&getState()->dataByteCode[3 * getState()->DC]) = number;
    getState()->DC++;
}