#ifndef ASSEMBLER_FIRSTPASS_H
#define ASSEMBLER_FIRSTPASS_H

#include "dissector.h"

int firstPass(char *fileName);

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

int handleDirectiveLabelFirstPass(DissectedLine line, DissectedDirective dissectedDirective);

#endif /*ASSEMBLER_FIRSTPASS_H*/
