#ifndef ASSEMBLER_ERRORLOG_H
#define ASSEMBLER_ERRORLOG_H

#include "state.h"
#include "structs.h"

/**
 * Log an error
 * @param lineNumber the line number in the assembly file
 * @param errorStr
 */
void logError(int lineNumber, char *errorStr);

/**
 * Print all logged errors to stderr
 */
void flush();

/**
 * Check how many errors were logged
 * @return the number of errors
 */
int numErrors();

void clearErrorLog();

/**
 * A macro representing an error clause/block.
 * Supports formatting.
 */
#define ERROR_RET(__msg) {\
                char _[200];\
                sprintf __msg;\
                logError(getLineNumber(), _);\
                return -1;\
            }

char* addressingTypeStr(enum AddressingType addressingType);
#endif /*ASSEMBLER_ERRORLOG_H*/
