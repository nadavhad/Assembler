#ifndef ASSEMBLER_EXTERNUSAGE_H
#define ASSEMBLER_EXTERNUSAGE_H

#include "constants.h"

/**
 * A container for all external symbol usage data
 */
typedef struct {
    char externLabel[MAX_LABEL_LENGTH];
    int usageAddress;
} ExternUsage;

/**
 * Log a usage of an external label/symbol
 */
int addUsage(char externLabel[MAX_LABEL_LENGTH], int usageAddress);

/**
 * Empty the usage recordings
 */
void clearExternUsagesTable();

/**
 * Expose the first Node for iteration purposes
 */
int startExternUsageIteration(void **iterator, ExternUsage *data);

/**
 * Expose the next Node for iteration purposes
 */
int getExternUsageNext(void **iterator, ExternUsage *data);


#endif /*ASSEMBLER_EXTERNUSAGE_H*/
