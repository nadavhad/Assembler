#include "parsing.h"

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERN ".extern"


int processAssemblyFile(char *string);

int firstPass(char *fileName);

int secondPass(char *fileName);

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

void initializeFirstPass();

int handleDirective(DissectedLine dissectedLine);


enum DirectiveType {
    DT_UNDEFINED,
    DT_DATA,
    DT_STRING,
    DT_ENTRY,
    DT_EXTERN
};

typedef struct {
    enum DirectiveType type;
    char directiveToken[10];
    char directiveArgs[80];
} DissectedDirective;

int handleDirectiveLabelFirstPass(DissectedLine line, DissectedDirective dissectedDirective);

int handleCommand(DissectedLine dissectedLine);

int verifyArguments(Operation *op, CommandTokens *commandTokens);

int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData);




#endif /*ASSEMBLER_ASSEMBLER_H*/
