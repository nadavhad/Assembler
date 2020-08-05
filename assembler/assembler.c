#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "assembler.h"
#include "state.h"
#include "../logging/errorlog.h"
#include "symbolTable.h"

int encodeCommandPass1(Operation *command, CommandTokens args, char encodedOpcode[9], int *opcodeLen);

/**
 * TODO:
 * Y d 0. Check if a label is valid (reserved words)
 * N d 1. Create symbol table (linked list. Data {Name, Value, Type, IsEntry})
 *      AddSymbol(name, value, type, entry) => error if label exists
 *      LookupSymbol(name)
 *      UpdateSymbol(name, value)
 *
 * N d 2. handleCmdLabelFirstPass
 * N d 3. handleDirectiveLabelFirstPass
 * Y i 4. Build opcode skeleton
 *     0. Return data for each argument (addressing, register value, value)
 *     a. Opcode, funct, ARE, Addressing, registers,
 *     b. Arguments
 * N d 5. Update symbol table
 * N d 6. Update code skeleton
 * N d 7. Update state (IC)
 *
 *
 * X. Handle directives
 */

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
        incLineNumber();
        if (dissectLabel(line, &dissectedLine) == -1) {
            continue;
        }

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
    initializeState();
}

/*------------------------
 *  Commands
 */
int handleCmdLabelFirstPass(DissectedLine dissectedLine) {
    if (addSymbol(dissectedLine.label, getState()->IC, ST_CODE, FALSE) != 0) {
        return -1;
    }
    return 0;
}

int handleCommand(DissectedLine dissectedLine) {
    CommandTokens commandTokens;
    Operation *command = malloc(sizeof(Operation));
    char encodedOpcode[9];
    int opcodeLen;
    if (dissectCommand(dissectedLine.command, &commandTokens) != 0) {
        return -1;
    }
    if (findOperation(commandTokens.command, command) != 0) {
        return -1;
    }
    if (verifyArguments(command, &commandTokens) != 0) {
        return -1;
    }
    if (encodeCommandPass1(command, commandTokens, encodedOpcode, &opcodeLen) != 0) {
        return -1;
    }

    memcpy(&(getState()->currentByteCode[getState()->IC]), encodedOpcode, opcodeLen);
    getState()->IC += opcodeLen;

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

/**
 * Returns 2 if an argument doesn't need a data byte, 0 on success, and -1 on failiure.
 */
int buildDataByte(Argument arg, EncodedArg *databyte) {
    enum bool isExternal = TRUE;
    databyte->data = (arg.addressing == AT_IMMEDIATE) ? arg.value.scalar : 0;
    switch (arg.addressing) {
        case AT_IMMEDIATE:
            databyte->A = 1;
            databyte->E = 0;
            databyte->R = 0;
            break;
        case AT_DIRECT:
            databyte->A = 0;
            /* internal: R; external: E */
            /* TODO: add external check */
            databyte->E = isExternal == TRUE ? 1 : 0;
            databyte->R = isExternal == TRUE ? 0 : 1;
            break;
        case AT_RELATIVE:
            databyte->A = 1;
            databyte->E = 0;
            databyte->R = 0;
            break;
        case AT_REGISTER:
            return 2;
        default:
            logError(getLineNumber(), "Something terrible has happened!!!");
            return -1;

    }
    return 0;
}

int encodeCommandPass1(Operation *command, CommandTokens args, char encodedOpcode[9], int *opcodeLen) {
    EncodedOperation operation;
    EncodedArg arg[2];
    int numArgs = 0;
    int retVal;
    /* start building bytecode*/
    operation.opcode = command->opcode;
    operation.funct = command->funct;
    if (args.numArgs < 1) { /* if we don't have args, empty all relevant fields */
        operation.destAddressing = 0;
        operation.destRegister = 0;
        operation.srcAddressing = 0;
        operation.srcRegister = 0;
    }
    if (args.numArgs == 1) {/* if we have only one arg, it's dest */
        numArgs = 1;
        operation.destRegister = args.arg1Data.reg;
        operation.destAddressing = args.arg1Data.addressing;

        retVal = buildDataByte(args.arg1Data, &arg[0]);
        if (retVal == -1) {
            return -1;
        } else if (retVal == 2) {
            numArgs = 0;
        }
    } else if (args.numArgs == 2) {/* if we have two args, then */
        numArgs = 2;
        operation.srcRegister = args.arg1Data.reg;
        operation.srcAddressing = args.arg1Data.addressing;

        retVal = buildDataByte(args.arg1Data, &arg[0]);
        if (retVal == -1) {
            return -1;
        } else if (retVal == 2) {
            numArgs--;
        }

        operation.destRegister = args.arg2Data.reg;
        operation.destAddressing = args.arg2Data.addressing;

        retVal = buildDataByte(args.arg2Data, &arg[numArgs - 1]);
        if (retVal == -1) {
            return -1;
        } else if (retVal == 2) {
            numArgs--;
        }
    }

    *opcodeLen = 3;
    memcpy(encodedOpcode, &operation, sizeof(operation));
    if (numArgs > 0) {
        memcpy(encodedOpcode + sizeof(operation), &arg[0], sizeof(EncodedArg));
        (*opcodeLen) += 3;
    }
    if (numArgs > 1) {
        memcpy(encodedOpcode + sizeof(operation) + sizeof(EncodedArg), &arg[1], sizeof(EncodedArg));
        (*opcodeLen) += 3;
    }
    return 0;
}

/* Split the input to the command and the parameters. Also removes redundant white spaces. */
int splitCommandAndParams(char *line, char *token, char *remainder) {
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
        logError(getLineNumber(), "Illegal comma");
        return -1;
    }
    /* If n > MAX_TOKENS it means we got more text than the maximum text expected*/
    if (n > MAX_TOKENS) {
        logError(getLineNumber(), "Extraneous text after end of command");
        return -1;
    }
    remainder[0] = 0;
    /* Concatenate all param tokens, making sure we have a separating comma between tokens. */
    for (i = 0; i < n - 1; i++) {
        int len = strlen(remainder);
        if ((len != 0) && (remainder[len - 1] != ',') && (parts[i][0] != ',')) {
            logError(getLineNumber(), "Missing comma");
            return -1;
        }
        strcat(remainder, parts[i]);
    }
    return 0;
}

int tokenizeParams(char *remainder, CommandTokens *parsedCommand) {
    int charIndex = 0;
    char *currArg;
    /* Start with all empty tokens. */
    parsedCommand->arg1[0] = parsedCommand->arg2[0] = 0;
    /* While we didn't get to the end of the remainder */
    currArg = parsedCommand->arg1;
    parsedCommand->numArgs = 0;
    while (*remainder != 0) {
        if (parsedCommand->numArgs > MAX_PARAMS - 1) { /* We have an extra parameter after the last valid token. */
            logError(getLineNumber(), "Extraneous text after end of command");
            return -1;
        }
        if (*remainder == ',') {
            /* Found a comma */
            if (charIndex == 0) {
                /* Beginning of token - Must be an error */
                if (parsedCommand->numArgs == 0) {
                    /* First token starts with a comma. */
                    logError(getLineNumber(), "Illegal comma");
                    return -1;
                }
                /* Some other (not first) token starts with a comma. We have two consecutive commas. */
                logError(getLineNumber(), "Multiple consecutive commas");
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
        logError(getLineNumber(), "Extraneous text after end of command");
        return -1;
    }
    if (charIndex > 0) {
        parsedCommand->numArgs++;
    }
    currArg[charIndex] = 0;
    return 0;
}


int verifyArguments(Operation *op, CommandTokens *commandTokens) {
    if (commandTokens->numArgs != op->numArgs) {
        char buf[200];
        sprintf(buf, "Wrong number of arguments for %s. Expected: %d Got: %d", op->name,
                op->numArgs, commandTokens->numArgs);
        logError(getLineNumber(), buf);
        return -1;
    }

    if (op->numArgs > 0) {
        int *addressing = op->srcAddressing;
        if (op->srcAddressing[0] == AT_UNSET) {
            addressing = op->destAddressing;
        }
        if (!matchesAddressing(addressing, commandTokens->arg1, &commandTokens->arg1Data)) {
            return -1;
        }
    }

    if (op->numArgs > 1) {
        if (!matchesAddressing(op->destAddressing, commandTokens->arg2, &commandTokens->arg2Data)) {
            return -1;
        }
    }
    return 0;
}

int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData) {
    int i;
    if (findArgumentAddressingType(arg, argData)) {
        return -1;
    }
    for (i = 0; validAddressingArr[i] != -1; i++) {
        if (validAddressingArr[i] == argData->addressing) {
            return 0;
        }
    }
    logError(getLineNumber(), "Wrong argument type");
    return -1;
}


int dissectCommand(char *commandStr, CommandTokens *parsedCommand) {
    /* 1. Check command structure (tokens, command, number of arguments)
     * 2. Split line to tokens: CommandTokens, argument1, argument2
     * */
    char remainder[MAX_LINE_LENGTH];
    if (splitCommandAndParams(commandStr, parsedCommand->command, remainder) < 0) {
        /* There was an error in the input */
        return -1;
    }
    if (tokenizeParams(remainder, parsedCommand) < 0) { /* There was an error in the input */
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

int handleDirectiveLabelFirstPass(DissectedLine dissectedLine) {
    /*TODO: handle .entry*/
    if (addSymbol(dissectedLine.label, getState()->DC, ST_DATA, FALSE) != 0) {
        return -1;
    }
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
        if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
            /* We got to the end of a file. */
            fclose(file);
            return 0;
        }
    }
}

int requiresLabel(const char *label) {
    /*TODO*/
    return 0;
}