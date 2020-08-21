#include <string.h>
#include "state.h"

State state;

/**
 * Get a ptr to the global state container
 * @return a ptr to the global state container
 */
State *getState() {
    return &state;
}

/**
 * Initializes the values of state
 * */
void initializeState() {
    getState()->lineNumber = 0;
    getState()->DC = 0;
    getState()->IC = 100;
    getState()->DCF = -1;
    getState()->ICF = -1;
    memset(getState()->currentByteCode, 0, sizeof(getState()->currentByteCode));
    memset(getState()->dataByteCode, 0, sizeof(getState()->dataByteCode));

}
/**
 * Get the current line number in the assembly file
 * */
int getLineNumber() {
    return getState()->lineNumber;
}
/**
* Increment the stored line number.
* (Advance to the next line in the assembly file)
*/
void incLineNumber() {
    getState()->lineNumber++;
}

/**
 * Add a command and increment IC accordingly.
 */
void addCommand(const char *encodedOpcode, int opcodeLen) {
    memcpy(&(getState()->currentByteCode[getState()->IC*3]), encodedOpcode, opcodeLen*3);
    /* Increment IC */
    getState()->IC += opcodeLen;
}
/**
* Add a data word and increment DC accordingly
*/
void addDataWord(long number) {
    memset(&(getState()->dataByteCode[3 * getState()->DC]), number < 0 ? 0xFF : 0, 3);
    *((long*)&getState()->dataByteCode[3 * getState()->DC]) = number;
    /* Increment DC */
    getState()->DC++;
}