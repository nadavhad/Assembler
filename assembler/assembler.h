#include "parsing.h"

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERN ".extern"


int processAssemblyFile(char *basefileName);

int firstPass(char *fileName);

int secondPass(char *fileName);

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

int handleDirective(DissectedDirective dissectedDirective);

int handleDirectiveLabelFirstPass(DissectedLine line, DissectedDirective dissectedDirective);

int handleCommand(DissectedLine dissectedLine);

int verifyArguments(Operation *op, CommandTokens *commandTokens);
/**
 *
 * @param validAddressingArr
 * @param arg
 * @param argData
 * @return
 */
int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData);




#endif /*ASSEMBLER_ASSEMBLER_H*/
