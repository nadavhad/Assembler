#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "assembler.h"
#include "state.h"
#include "../logging/errorlog.h"
#include "symbolTable.h"
#include "macros.h"
#include "outfile.h"
#include "externusage.h"


int encodeCommand(Operation *command, CommandTokens args, char *encodedOpcode, int *opcodeLen);

int writeOutputFiles(char*);

/**
 * TODO: make sure 2's complement is not machine depepndent x
 * ASK:
 *    Can we use a contiguous array for data as well as code?
 *    Are .data numbers limited to char size? if so - what is the range? [-254, 255]/[-127, 128]/else
 *    Should we support escape sequences for .string? if so - which escapes? "\""? "\n"` etc.
 *
 * TODO:
 * N d Add .as to input files
 * N d Check that each input line length is under than 80
 * Y d 0. Check if a label is valid (reserved words)
 * N d 1. Create symbol table (linked list. Data {Name, Value, Type, IsEntry})
 *   d  AddSymbol(name, value, type, entry) => error if label exists
 *   d  LookupSymbol(name)
 *   d  UpdateSymbol(name, value)
 * N d 2. handleCmdLabelFirstPass
 * N d 3. handleDirectiveLabelFirstPass
 * Y d 4. Build opcode skeleton
 *   d  0. Return data for each argument (addressing, register value, value)
 *   d  a. Opcode, funct, ARE, Addressing, registers,
 *   d  b. Arguments
 * N d 5. Update symbol table
 * N d 6. Update code skeleton
 * N d 7. Update state (IC)
 *   d 8. Handle directives
 * N d   9. Check directive type
 * N d  10. For .entry, extern
 * N d     10.1 Parse line
 * N d     10.2 validate label  (and no more)
 * N d     10.3 for .entry - continue
 * N d          Validate no label BEFORE
 * N d     10.4. For .extern - Add to symbol table (value 0, as extern) - if symbol already exists (only as an extern(?)) it's OK. Do nothing.
 * N d          Validate no label BEFORE
 *   d 11. .data, .string
 * Y d    Later use that in IMMEDIATE addressing as well
 * Y d    11.1 Add data array to state
 * Y d    11.2 .data - parse data (comma separated numbers), add to Data array, increment DC by data size
 *   d         Loop
 *   d              strtol, check we got a number
 *   d              Strip, make sure the string is empty or starts with ','
 *   d              "eat" the comma
 * Y d    11.3 .string - parse data (quoted string), add to Data array, add terminating 0, increment DC by data size + 1
 *   d         Strip
 *   d         Make sure that starts and ends with "
 *   d         Remove quotes
 *   d         Make sure no quotes in the string
 * N d 12. Save ICF, DCF
 *
 * Phase 2
 * Y d 1. Write externUsageTable
 * Y d    Make sure to free wherever other lists are freed.
 * N d 2. Write second pass skeleton
 * N d       (reset dc[0], ic [100])
 * N d       for each line
 * N d         skip .extern, .string, .data
 * N d         handle .entry
 * N d           add isEntry to symbol table
 * N d         or
 * N d         handle command
 * N d           dissectLabel
 * N d           dissectCommand
 * N d           findOperation
 * N d           verifyArguments
 * N d           encodeCommand
 * N d           addCommand
 * N d           for external symbols - add to external usage table
 * Y d             implement for buildDataByte
 * Y d                AT_DIRECT
 * Y d                AT_RELATIVE
 * X   3. Create output files
 * Y       code
 * N       entry
 * N       externals
 *
 * N d X Write tests (for stage 1)
 * Y   X Write tests (for stage 2)
 *   d  X. Free lists (error log, symbol table, etc.)
 *
 *     X. Document!!!!!!
 */

int processAssemblyFile(char *fileName) {
    /* if the first pass failed (regardless of input validity) , exit */
    if (firstPass(fileName) != 0) {
        return -1;
    }
    /* if we had any input errors during first pass, print them and exit */
    if (numErrors() > 0) {
        flush();
        return -1;
    }
    /* if the second pass failed (regardless of input validity) , exit */
    if (secondPass(fileName) != 0) {
        return -1;
    }
    /* if we had any input errors during second pass, print them and exit */
    if (numErrors() > 0) {
        flush();
        return -1;
    }

    /* Write output files */
    if (writeOutputFiles(fileName) != 0) {
        flush();
        return -1;
    }
    return 0;
}
/* ************************
 *  First pass
 */
/**
 * Strips leading and trailing whitespace from a string.
 * Copies the result to stripped
 * @param rawStr the raw string
 * @param stripped the address to copy the stripped string to. Must be Non-NULL.
 * @return 0 on success, -1 on failure.
 */
int stripWhiteSpaces(char *rawStr, char stripped[MAX_LINE_LENGTH]) {
    char *end;
    /* empty output space */
    memset(stripped, 0, MAX_LINE_LENGTH);
    /* advance start ptr until we reach a non-whitespace char */
    while (WHT(*rawStr) && !EOS(*rawStr) && !END(*rawStr)) {
        rawStr++;
    }
    strncpy(stripped, rawStr, MAX_LINE_LENGTH);
    end = stripped + strlen(stripped) - 1;
    /* advance from end, place NUL char instead of each whitespace */
    while (WHT(*end)) {
        *end = 0;
        end--;
    }
    return 0;
}

int getDirectiveType(DissectedLine dissectedLine, DissectedDirective *directive) {
    int index = 0;
    char *iterator = dissectedLine.command;
    /* Tokenize the line into: directive and directive arguments */
    /* Find the directive token. */
    memset(directive->directiveToken, 0, sizeof(directive->directiveToken));
    while (!END(*iterator) && !WHT(*iterator) && (index < (sizeof(directive->directiveToken) - 1))) {
        directive->directiveToken[index] = *iterator;
        index++;
        iterator++;
    }
    /* Store the directive "argument" - the rest of the line. */
    stripWhiteSpaces(iterator, directive->directiveArgs);

    /* Find the directive type */
    if (strcmp(directive->directiveToken, DATA) == 0) {
        directive->type = DT_DATA;
    } else if (strcmp(directive->directiveToken, STRING) == 0) {
        directive->type = DT_STRING;
    } else if (strcmp(directive->directiveToken, ENTRY) == 0) {
        directive->type = DT_ENTRY;
    } else if (strcmp(directive->directiveToken, EXTERN) == 0) {
        directive->type = DT_EXTERN;
    } else {
        directive->type = DT_UNDEFINED;
        ERROR_RET((_, "Invalid directive: %s", directive->directiveToken));
    }
    return 0;
}

int firstPass(char *fileName) {
    FILE *file;
    char line[MAX_LINE_LENGTH + 5];
    DissectedLine dissectedLine;
    DissectedDirective dissectedDirective;
    file = fopen(fileName, "r");
    /* if we can't open the file, no point in continuing */
    if (file == NULL) {
        perror(fileName);
        return -1;
    }

    initializeState();
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH + 5, file) == NULL) {
            /* We got to the end of a file: close file, log IC and DC values */
            fclose(file);
            getState()->ICF = getState()->IC;
            getState()->DCF = getState()->DC;
            incrementDataSymbolsOffset(getState()->ICF);
            return 0;
        }
        if (strlen(line) > MAX_LINE_LENGTH) {
            ERROR_RET((_, "Line is too long"))
        }
        incLineNumber();
        /* if the label is invalid, we have nothing to do with the line */
        if (dissectLabel(line, &dissectedLine) == -1) {
            continue;
        }

        switch (dissectedLine.lineType) {
            /* ignore empty line or comment */
            case LT_COMMENT:
                break;
                /* handle commands */
            case LT_COMMAND:
                if (handleCmdLabelFirstPass(dissectedLine) == 0) {
                    handleCommand(dissectedLine);
                }
                break;
                /* handle directives */
            case LT_DIRECTIVE:
                if (getDirectiveType(dissectedLine, &dissectedDirective) == 0) {
                    if (handleDirectiveLabelFirstPass(dissectedLine, dissectedDirective) == 0) {
                        handleDirective(dissectedDirective);
                    }
                }
                break;
        }
    }
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
    Operation command;
    char encodedOpcode[9];
    int opcodeLen;
    if (dissectCommand(dissectedLine.command, &commandTokens) != 0) {
        return -1;
    }
    if (findOperation(commandTokens.command, &command) != 0) {
        return -1;
    }
    if (verifyArguments(&command, &commandTokens) != 0) {
        return -1;
    }

    if (encodeCommand(&command, commandTokens, encodedOpcode, &opcodeLen) != 0) {
        return -1;
    }

    addCommand(encodedOpcode, opcodeLen);

    return 0;
}

/**
 * Returns 2 if an argument doesn't need a data byte, 0 on success, and -1 on failure.
 */
int buildDataByte(Argument arg, EncodedArg *databyte) {
    SymbolData symbolData;
    memset(databyte, 0, sizeof(*databyte));
    switch (arg.addressing) {
        case AT_IMMEDIATE:
            databyte->A = 1;
            databyte->E = 0;
            databyte->R = 0;
            databyte->data = arg.value.scalar;
            break;
        case AT_DIRECT:
            databyte->A = 0;
            /* internal: R; external: E */
            if (isSymbolTableComplete() == FALSE) {
                break;
            }
            if (lookUp(arg.value.symbol, &symbolData) == -1) {
                ERROR_RET((_, "Cannot find label: %s", arg.value.symbol));
            }

            if (symbolData.type == ST_EXTERNAL) {
                addUsage(symbolData.name, getState()->IC);
                databyte->E = 1;
                databyte->R = 0;
                databyte->data = 0;
            } else {
                databyte->E = 0;
                databyte->R = 1;
                databyte->data = symbolData.value;
            }
            break;
        case AT_RELATIVE:
            databyte->A = 1;
            databyte->E = 0;
            databyte->R = 0;
            if (isSymbolTableComplete() == FALSE) {
                break;
            }
            if (lookUp(arg.value.symbol, &symbolData) == -1) {
                ERROR_RET((_, "Cannot find label: %s", arg.value.symbol));
            }
            if (symbolData.type == ST_EXTERNAL) {
                ERROR_RET((_, "External labels are not supported for relative addressing (%s)",
                        arg.value.symbol));
            }
            databyte->data = symbolData.value - getState()->IC;
            break;
        case AT_REGISTER:
            return 2;
        default:
            logError(getLineNumber(), "Something terrible has happened!!!");
            return -1;

    }
    return 0;
}

int encodeCommand(Operation *command, CommandTokens args, char *encodedOpcode, int *opcodeLen) {
    EncodedOperation operation;
    EncodedArg arg[2];
    int numArgs = 0;
    int retVal;
    memset(encodedOpcode, 0, 9);
    memset(&operation, 0, sizeof(operation));
    memset(&arg[0], 0, sizeof(arg[0]));
    memset(&arg[1], 0, sizeof(arg[1]));
    /* start building bytecode*/
    operation.A = 1;
    operation.R = 0;
    operation.E = 0;
    operation.opcode = command->opcode;
    operation.funct = command->funct;
    if (args.numArgs < 1) { /* if we don't have args, empty all relevant fields */
        operation.destAddressing = 0;
        operation.destRegister = 0;
        operation.srcAddressing = 0;
        operation.srcRegister = 0;
    }
    if (args.numArgs == 1) { /* if we have only one arg, it's dest */
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

    *opcodeLen = 1;
    memcpy(encodedOpcode, &operation, 3);
    if (numArgs > 0) {
        memcpy(encodedOpcode + 3, &arg[0], 3);
        (*opcodeLen)++;
    }
    if (numArgs > 1) {
        memcpy(encodedOpcode + 6, &arg[1], 3);
        (*opcodeLen)++;
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
        ERROR_RET((_, "Extraneous text after end of command. Too many parameters"));
    }
    if (charIndex > 0) {
        parsedCommand->numArgs++;
    }
    currArg[charIndex] = 0;
    return 0;
}

int verifyArguments(Operation *op, CommandTokens *commandTokens) {
    if (commandTokens->numArgs != op->numArgs) {
        ERROR_RET((_, "Wrong number of arguments for %s. Expected: %d Got: %d",
                op->name, op->numArgs, commandTokens->numArgs));
    }

    if (op->numArgs > 0) {
        if (op->srcAddressing[0] == AT_UNSET) {
            if (matchesAddressing(op->destAddressing, commandTokens->arg1, &commandTokens->arg1Data) != 0) {
                return -1;
            }
        } else {
            if (matchesAddressing(op->srcAddressing, commandTokens->arg1, &commandTokens->arg1Data) != 0) {
                return -1;
            }
        }
    }

    if (op->numArgs > 1) {
        if (matchesAddressing(op->destAddressing, commandTokens->arg2, &commandTokens->arg2Data) != 0) {
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
    ERROR_RET((_, "Wrong argument type: %d", argData->addressing));
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
}

/*----------------------
 *  Directives
 */
int handleDirective(DissectedDirective dissectedDirective) {
    /* Handle .entry, .extern */
    if ((dissectedDirective.type == DT_ENTRY) || (dissectedDirective.type == DT_EXTERN)) {
        if (validateLabel(dissectedDirective.directiveArgs) != 0) {
            ERROR_RET((_, "%s requires a valid label. Got %s instead",
                    (dissectedDirective.type == DT_ENTRY) ? ENTRY : EXTERN,
                    dissectedDirective.directiveArgs))
        }
    }
    if (dissectedDirective.type == DT_ENTRY) {
        return 0;
    }
    if (dissectedDirective.type == DT_EXTERN) {
        addSymbol(dissectedDirective.directiveArgs, 0, dissectedDirective.type, FALSE);
        return 0;
    }

    if (dissectedDirective.type == DT_STRING) {
        char strippedBuf[MAX_LINE_LENGTH];
        char *stripped = strippedBuf;
        stripWhiteSpaces(dissectedDirective.directiveArgs, stripped);
        if (stripped[0] != '\"') {
            /* if the first character after stripping isn't ", then there either isn't one
             * or there are chars before it. */
            ERROR_RET((_, "Cannot find starting \'\"\'"));
        }
        if (stripped[strlen(stripped) - 1] != '\"') {
            /* if the last character after stripping isn't ", this is an error. */
            ERROR_RET((_, "Cannot find closing \'\"\'"));
        }

        stripped++; /* skip the first quote */
        while (*stripped != '\"') {
            /* until we reach the end quote */
            addDataWord(*stripped);
            stripped++;
        }
        /* add terminating zero */
        addDataWord(0);
        return 0;
    }

    if (dissectedDirective.type == DT_DATA) {
        char strippedBuf[MAX_LINE_LENGTH];
        char *stripped = strippedBuf;
        long number;
        stripWhiteSpaces(dissectedDirective.directiveArgs, stripped);
        while (stripped[0] != 0) {
            char iteratorBuf[MAX_LINE_LENGTH];
            char *iterator = iteratorBuf;
            strcpy(iteratorBuf, stripped);
            number = strtol(iterator, &iterator, 10);
            if (iterator == iteratorBuf) {
                ERROR_RET((_, "Entries in \".data\" must be integer numbers"));
            }
            addDataWord(number);
            stripWhiteSpaces(iterator, stripped);
            if (stripped[0] == ',') {
                stripped++;
                continue;
            } else if (stripped[0] == 0) {
                /* finished data parsing */
                return 0;
            } else {
                /* error - expected end or ','. */
                ERROR_RET((_, "Unexpected chars: "))
            }
        }
        /* empty .data : warning? */
        return 0;
    }
/* invalid DirectiveType */
    ERROR_RET((_, "Unexpected value: "));

}

int handleDirectiveLabelFirstPass(DissectedLine dissectedLine, DissectedDirective dissectedDirective) {
    enum bool isEntry = FALSE;
    /* Any label before .entry/.extern is ignored. We issue a warning to the user. */
    if ((dissectedDirective.type == DT_ENTRY) || (dissectedDirective.type == DT_EXTERN)) {
        if (strlen(dissectedLine.label) == 0) {
            printf("%d: Warning: There is a label before %s - it will be ignored\n", getLineNumber(),
                   (dissectedDirective.type == DT_ENTRY) ? ".entry" : ".extern");
        }
        return 0;
    }

    if (addSymbol(dissectedLine.label, getState()->DC, ST_DATA, isEntry) != 0) {
        return -1;
    }
    return 0;
}

/*************************
 *  Second pass
 */
int secondPass(char *fileName) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    DissectedLine dissectedLine;
    DissectedDirective dissectedDirective;
    file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open input file: ");
        return -1;
    }
    getState()->lineNumber = 0;
    getState()->IC = 100;
    getState()->DC = 0;
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
            /* We got to the end of a file. */
            fclose(file);
            return 0;
        }
        incLineNumber();
        /* if the label is invalid, we have nothing to do with the line */
        if (dissectLabel(line, &dissectedLine) == -1) {
            continue;
        }

        switch (dissectedLine.lineType) {
            /* ignore empty line or comment */
            case LT_COMMENT:
                break;
                /* handle commands */
            case LT_COMMAND:
                handleCommand(dissectedLine);
                break;
                /* handle directives */
            case LT_DIRECTIVE:
                if (getDirectiveType(dissectedLine, &dissectedDirective) == -1) {
                    return -1;
                }
                if (dissectedDirective.type != DT_ENTRY) {
                    return 0;
                }
                if (setEntrySymbol(dissectedDirective.directiveToken) == -1) {
                    return -1;
                }
                break;
        }
    }
}

/*************************
 *  Write output
 */
int writeOutputFiles(char* basefilename) {
    /* TODO: Implement! */
    createCodeOutputFile(basefilename);
    return 0;
}
