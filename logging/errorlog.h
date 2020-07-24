#ifndef ASSEMBLER_ERRORLOG_H
#define ASSEMBLER_ERRORLOG_H
#define println(String) printf("%s\n", String)
void logError(int lineNumber, char *errorStr);
void flush();
int numErrors();
#endif /*ASSEMBLER_ERRORLOG_H*/
