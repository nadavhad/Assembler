#ifndef ASSEMBLER_ERRORLOG_H
#define ASSEMBLER_ERRORLOG_H
/**
 * Log an error
 * @param lineNumber
 * @param errorStr
 */
void logError(int lineNumber, char *errorStr);
/**
 * Print all logged errors to stderr
 */
void flush();
/**
 * Check how many errors were logged
 * @return
 */
int numErrors();
#endif /*ASSEMBLER_ERRORLOG_H*/
