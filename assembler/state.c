#include <stdlib.h>
#include "structs.h"
#include "../logging/errorlog.h"

State state;

State *getState() {
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

ByteCode *getCurrentBytecode() {
    return getState()->currentByteCode;
}

DataByte *initDataByte(unsigned int A, unsigned int E, unsigned int R, Data data) {
    DataByte *byte = malloc(sizeof(DataByte));
    byte->A = A;
    byte->E = E;
    byte->R = R;
    byte->data = data;
    return byte;
}

int isInternal(const char *name) {
    return 0;
}

int setArgumentBytecode(Argument *arg) {
    DataByte *argbytecode = NULL;
    switch (arg->addressing) {
        case AT_IMMEDIATE:
            argbytecode = initDataByte(1, 0, 0, arg->value);
            break;
        case AT_DIRECT:
            argbytecode = initDataByte(0, isInternal(arg->value.symbol) ? 0 : 1,
                                       isInternal(arg->value.symbol) ? 1 : 0, arg->value);
            break;
        case AT_RELATIVE:
            argbytecode = initDataByte(1, 0, 0, arg->value);
            break;
        case AT_REGISTER:
            return 0;
        case AT_UNSET:
            logError(getLineNumber(), "Unexpected arg type!!!!!");
            return -1;
    }
    if (arg->position == AP_DEST) {
        getCurrentBytecode()->srcAddressing = arg->addressing;
        getState()->dataBytes[1] = argbytecode;
        return 0;
    }
    if (arg->position == AP_SRC) {
        getCurrentBytecode()->destAddressing = arg->addressing;
        getState()->dataBytes[0] = argbytecode;
        return 0;
    }
    return -1;
}