#ifndef SHAREDFOLDER_STATE_H
#define SHAREDFOLDER_STATE_H



void initializeState();
State *getState();
int getLineNumber();
void incLineNumber();
ByteCode *getCurrentBytecode();
#endif /*SHAREDFOLDER_STATE_H*/
