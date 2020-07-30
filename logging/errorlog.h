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

/**
 * A macro representing an error clause/block based on an illegal argument
 */
#define ERROR_ARG(msg, arg) {\
                char errormsg[100] = msg;\
                strcat(errormsg, arg);\
                logError(-1, errormsg);\
                return -1;\
            }
#endif /*ASSEMBLER_ERRORLOG_H*/
