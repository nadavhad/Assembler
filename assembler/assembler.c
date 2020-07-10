#include <stdio.h>
#include "assembler.h"

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

int main(int argc, char **argv) {
    int i = 0;
    for (i = 1; i < argc; ++i) {
        processAssemblyFile(argv[i]);
    }

    return 0;
}

int processAssemblyFile(char *fileName) {
    firstPass(fileName);
    secondPass(fileName);
    return 0;
}


/*************************
 *  First pass
 */
int firstPass(char *fileName) {
    /* TODO(nadav): Open file*/
    FILE *file;
    char line[MAX_LINE_LENGTH];
    DissectedLine dissectedLine;
    initializeFirstPass();
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            /* We got to the end of a file. */
            return 0;
        }
        dissectLabel(line, &dissectedLine);
        switch (dissectedLine.lineType) {
            case LT_COMMENT:
                break;
            case LT_COMMAND:
                handleCmdLabelFirstPass(dissectedLine);
                handleCommand(dissectedLine);
                break;
            case LT_DIRECTIVE:
                handleDirectiveLabelFirstPass(dissectedLine);
                handleDirective(dissectedLine);
                break;
        }
    }
    /*TODO:
     * 1. If found errors: print and stop
     * 2. Store ICF, DCF
     * 3. Update data labels in symbol table
     * */
}

void initializeFirstPass() {
    /*TODO: Implement*/
}
/*------------------------
 *  Commands
 */
int handleCmdLabelFirstPass(DissectedLine dissectedLine) {
    return 0;
    /*TODO: Implement*/
}

int dissectLabel(char* rawLine, DissectedLine* dissectedLine) {
    return -2;
    /*TODO: Implement*/
}


int handleCommand(DissectedLine dissectedLine) {
    CommandTokens commandTokens;
    Command command;
    if (dissectCommand(dissectedLine.command, &commandTokens) != 0) {
        return -1;
    }
    if (findCommandInTable(commandTokens, command) != 0) {
        return -1;
    }
    /*
     * TODO:
     * 1. Look for the command in table
     * 2. Verify number of arguments
     * 3. Verify argument addressing (by command)
     * 4. Compute command size (including arguments) = L
     * 5. Build binary command+arguments code
     * 6. Save IC, L with binary code
     * 7. Increment IC accordingly
     */
    return 0;
}

int dissectCommand(char *commandStr, CommandTokens *parsedCommand) {
    /* 1. Check command structure (toekns, command, number of arguments)
     * 2. Split line to tokens: CommandTokens, argument1, argument2
     * */
    return 0;
    /* TODO: Implement*/
}

/*----------------------
 *  Directives
 */
int handleDirective(DissectedLine dissectedLine) {
    return 0;
    /* TODO: Implement*/

}

int handleDirectiveLabelFirstPass(DissectedLine line) {
    return 0;
    /*TODO: Implement*/
}

int findCommandInTable(CommandTokens tokens, Command command) {
    /*TODO(nadav): Implement*/
    return 0;
}

/*************************
 *  Sceond pass
 */
int secondPass(char *fileName) {
    /* TODO(nadav): Open file*/
    FILE *file;
    char line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            /* We got to the end of a file. */
            return 0;
        }
    }
}

