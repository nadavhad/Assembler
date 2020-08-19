#ifndef ASSEMBLER_EXTERNUSAGE_H
#define ASSEMBLER_EXTERNUSAGE_H

#include "constants.h"

typedef struct {
    char externLabel[MAX_LABEL_LENGTH];
    int usageAddress;
} ExternUsage;

int addUsage(char externLabel[MAX_LABEL_LENGTH], int usageAddress);

void clearExternUsagesTable();

int startExternUsageIteration(void **iterator, ExternUsage *data);

int getExternUsageNext(void **iterator, ExternUsage *data);



#endif /*ASSEMBLER_EXTERNUSAGE_H*/
