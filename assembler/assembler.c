#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "../logging/errorlog.h"

int main(int argc, char **argv) {
    int i = 0;
    for (i = 1; i < argc; ++i) {
        processAssemblyFile(argv[i]);
    }

    return 0;
}

int processAssemblyFile(char *fileName) {
    if (firstPass(fileName) != 0) {
        return -1;
    }
    if (numErrors() > 0) {
        flush();
        return -1;
    }
    if (secondPass(fileName) != 0) {
        return -1;
    }
    if (numErrors() > 0) {
        flush();
        return -1;
    }
    return 0;
}


/*************************
 *  First pass
 */
int firstPass(char *fileName) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    DissectedLine dissectedLine;
    int lineNumber = 0;
    file = fopen(fileName, "r");
    if (file == NULL) {
        perror(fileName);
        return -1;
    }
    initializeFirstPass();

    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
            /* We got to the end of a file. */
            fclose(file);
            return 0;
        }
        lineNumber++;
        dissectLabel(line, &dissectedLine);
        switch (dissectedLine.lineType) {
            case LT_COMMENT:
                break;
            case LT_COMMAND:
                handleCmdLabelFirstPass(dissectedLine);
                handleCommand(dissectedLine, lineNumber);
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

int dissectLabel(char *rawLine, DissectedLine *dissectedLine) {
    dissectedLine->label[0] = 0;
    strncpy(dissectedLine->command, rawLine, 200);
    dissectedLine->lineType = LT_COMMAND;
    return 0;
    /*TODO: Implement*/
}


int handleCommand(DissectedLine dissectedLine, int lineNumber) {
    CommandTokens commandTokens;
    Command command;
    if (dissectCommand(dissectedLine.command, lineNumber, &commandTokens) != 0) {
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

/* Split the input to the command and the parameters. Also removes redundant white spaces. */
int splitCommandAndParams(char *line, int lineNumber, char *token, char *remainder) {
    char parts[MAX_TOKENS + 1][MAX_LINE_LENGTH];
    int i;
    /*
     * We want to get rid of any redundant whitespaces, and get the command string + comma separated
     * parameters (without whitespaces).
     * Every valid command contains at most 4 tokens (command, param1, comma, param2).
     * We read up to seven tokens in order to detect any extraneous tokens.
     **/
    int n = sscanf(line, "%s%s%s%s%s%s", token, parts[0], parts[1], parts[2], parts[3], parts[4]);
    if (n == -1) { /* No token detected. This is an empty string. */
        return -1;
    }
    /* Check if the last character in the command token is a comma */
    if (token[strlen(token) - 1] == ',') {
        logError(lineNumber, "Illegal comma");
        return -1;
    }
    /* If n > MAX_TOKENS it means we got more text than the maximum text expected*/
    if (n > MAX_TOKENS) {
        logError(lineNumber, "Extraneous text after end of command");
        return -1;
    }
    remainder[0] = 0;
    /* Concatenate all param tokens, making sure we have a separating comma between tokens. */
    for (i = 0; i < n - 1; i++) {
        int len = strlen(remainder);
        if ((len != 0) && (remainder[len - 1] != ',') && (parts[i][0] != ',')) {
            logError(lineNumber, "Missing comma");
            return -1;
        }
        strcat(remainder, parts[i]);
    }
    return 0;
}

int tokenizeParams(char *remainder, int lineNumber, CommandTokens *parsedCommand) {
    int charIndex = 0;
    char *currArg;
    /* Start with all empty tokens. */
    parsedCommand->arg1[0] = parsedCommand->arg2[0] = 0;
    /* While we didn't get to the end of the remainder */
    currArg = parsedCommand->arg1;
    parsedCommand->numArgs = 0;
    while (*remainder != 0) {
        if (parsedCommand->numArgs > MAX_PARAMS - 1) { /* We have an extra parameter after the last valid token. */
            logError(lineNumber, "Extraneous text after end of command");
            return -1;
        }
        if (*remainder == ',') {
            /* Found a comma */
            if (charIndex == 0) {
                /* Beginning of token - Must be an error */
                if (parsedCommand->numArgs == 0) {
                    /* First token starts with a comma. */
                    logError(lineNumber, "Illegal comma");
                    return -1;
                }
                /* Some other (not first) token starts with a comma. We have two consecutive commas. */
                logError(lineNumber, "Multiple consecutive commas");
                return -1;
            }
            /* Closing the token */
            currArg[charIndex] = 0;
            charIndex = 0;
            parsedCommand->numArgs++;
            currArg = parsedCommand->arg2;
            remainder++;
        } else {
            /* Plain character. Add to current param*/
            currArg[charIndex] = *remainder;
            charIndex++;
            remainder++;
        }
    }
    /* We have an extra parameter after the last valid token. */
    if (parsedCommand->numArgs > MAX_PARAMS - 1) {
        logError(lineNumber, "Extraneous text after end of command");
        return -1;
    }
    if (charIndex > 0) {
        parsedCommand->numArgs++;
    }
    currArg[charIndex] = 0;
    return 0;
}

int dissectCommand(char *commandStr, int lineNumber, CommandTokens *parsedCommand) {
    /* 1. Check command structure (tokens, command, number of arguments)
     * 2. Split line to tokens: CommandTokens, argument1, argument2
     * */
    char remainder[MAX_LINE_LENGTH];
    if (splitCommandAndParams(commandStr, lineNumber, parsedCommand->command, remainder) < 0) {
        /* There was an error in the input */
        return -1;
    }
    if (tokenizeParams(remainder, lineNumber, parsedCommand) < 0) { /* There was an error in the input */
        return -1;
    }
    return 0;
    /* TODO(nadav): Implement*/
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
    /*TODO(nadav): Implement. Possible duplicate of findOperation?*/
    return 0;
}

/*************************
 *  Second pass
 */
int secondPass(char *fileName) {
    /* TODO(nadav): Open file*/
    FILE *file;
    char line[MAX_LINE_LENGTH];
    file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open input file: ");
        return -1;
    }
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            /* We got to the end of a file. */
            fclose(file);
            return 0;
        }
    }
}

