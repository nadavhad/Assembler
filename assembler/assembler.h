#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

/* Maximum length of a line from the input */
#define MAX_LINE_LENGTH 200
#define MAX_CMD_LENGTH 10
#define MAX_LABEL_LENGTH 31

enum LineType {
    LT_COMMENT,
    LT_COMMAND,
    LT_DIRECTIVE
};
typedef struct {
    char label[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    enum LineType lineType; /* TODO(yotam): Return line type*/
} DissectedLine;

typedef struct {
  char command[MAX_CMD_LENGTH];
  char arg1[MAX_LINE_LENGTH];
  char arg2[MAX_LINE_LENGTH];
} CommandTokens;

typedef struct {
    /*TODO(nadav): Define contents*/
} Command;

/**
 * Splits a line of assembly code into a struct containing the label and command (with its args)
 * and also identifies the coarse line type.
 * rawLine - Input line
 * dissectedLine - the dissected input line
 * returns 0 on success.
 */
int dissectLabel(char* rawLine, DissectedLine* dissectedLine);

/**
 * Splits a command string into a command and its arguments
 * @return
 */
int dissectCommand(char *commandStr, CommandTokens *parsedCommand);


/* Maximum amount of parameters in any function */
#define MAX_PARAMS 3
/* Maximum amount of tokens from any line*/
#define MAX_TOKENS 6

int processAssemblyFile(char *string);

int firstPass(char *fileName);

int secondPass(char *fileName);

int handleCmdLabelFirstPass(DissectedLine dissectedLine);

void initializeFirstPass();

int handleDirective(DissectedLine dissectedLine);

int handleDirectiveLabelFirstPass(DissectedLine line);

int handleCommand(DissectedLine dissectedLine);

int dissectCommand(char *commandStr, CommandTokens *parsedCommand);

int findCommandInTable(CommandTokens tokens, Command command);


#endif /*ASSEMBLER_ASSEMBLER_H*/
