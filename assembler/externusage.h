#ifndef ASSEMBLER_EXTERNUSAGE_H
#define ASSEMBLER_EXTERNUSAGE_H

#include "constants.h"

typedef struct {
    char externLabel[MAX_LABEL_LENGTH];
    int usageAddress;
} ExternUsage;

int addUsage(char externLabel[MAX_LABEL_LENGTH], int usageAddress);

void clearExternUsagesTable();

#endif /*ASSEMBLER_EXTERNUSAGE_H*/
