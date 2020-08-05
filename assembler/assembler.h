#include "parsing.h"

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H


int processAssemblyFile(char *string);

int firstPass(char *fileName);

int secondPass(char *fileName);

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

void initializeFirstPass();

int handleDirective(DissectedLine dissectedLine);


typedef struct {
    int x;
} DirectiveType;

int handleDirectiveLabelFirstPass(DissectedLine line, DirectiveType directiveType);

int handleCommand(DissectedLine dissectedLine);

int verifyArguments(Operation *op, CommandTokens *commandTokens);

int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData);




#endif /*ASSEMBLER_ASSEMBLER_H*/
