#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "assembler.h"
#include "state.h"
#include "errorLog.h"
#include "symbolTable.h"
#include "parsing.h"
#include "externUsage.h"
#include "dissector.h"

static int verifyArguments(Operation *op, CommandTokens *commandTokens);

static int encodeCommand(Operation *command, CommandTokens args, char *encodedOpcode, int *opcodeLen);

/*
 * Builds a struct containing the values of an argument data "word"
 * @return 2 if an argument doesn't need a data byte, 0 on success, and -1 on failure.
 */
static int buildDataByte(Argument arg, int databyteOffset, EncodedArg *databyte);

static int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData);

/*
 * Flatten directory structure.
 * Make should work by just typing 'make'
 * Document
 * i Y  assembler
 *   N  constants.h
 *   N  dissector
 *   Y  errorlog
 *   Y  externusage
 *   N  firstpass
 *   N  main.c
 *   Y  outfile
 * d -  parsing
 *   Y  secondpass
 *   N  state
 *   N  symboltable
 * d -  types.h
 */

int handleCommand(DissectedLine dissectedLine) {
    CommandTokens commandTokens;
    Operation command;
    char encodedOpcode[9];
    int opcodeLen;
    /* split the command string into command + args tokens */
    if (dissectCommand(dissectedLine.command, &commandTokens) != 0) {
        return -1;
    }
    /* find out what command this is */
    if (findOperation(commandTokens.command, &command) != 0) {
        return -1;
    }
    /* assert that the args fit the command */
    if (verifyArguments(&command, &commandTokens) != 0) {
        return -1;
    }
    /* translate the command into machine code */
    if (encodeCommand(&command, commandTokens, encodedOpcode, &opcodeLen) != 0) {
        return -1;
    }
    /* save the machine code */
    addCommand(encodedOpcode, opcodeLen);
    return 0;
}


static int buildDataByte(Argument arg, int databyteOffset, EncodedArg *databyte) {
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
                addUsage(symbolData.name, getState()->IC + databyteOffset);
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
        default: ERROR_RET((_, "Illegal addressing type. Something terrible has happened!!!"));
    }
    return 0;
}

static int encodeCommand(Operation *command, CommandTokens args, char *encodedOpcode, int *opcodeLen) {
    EncodedOperation operation;
    EncodedArg arg[2];
    int numArgs = 0;
    int retVal;
    /* TODO: magic number */
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

        retVal = buildDataByte(args.arg1Data, 1, &arg[0]);
        if (retVal == -1) {
            return -1;
        } else if (retVal == 2) {
            numArgs = 0;
        }
    } else if (args.numArgs == 2) {/* if we have two args, then first is src and second is dest*/
        numArgs = 2;
        operation.srcRegister = args.arg1Data.reg;
        operation.srcAddressing = args.arg1Data.addressing;

        retVal = buildDataByte(args.arg1Data, 1, &arg[0]);
        if (retVal == -1) {
            return -1;
        } else if (retVal == 2) {
            numArgs--;
        }

        operation.destRegister = args.arg2Data.reg;
        operation.destAddressing = args.arg2Data.addressing;

        retVal = buildDataByte(args.arg2Data, numArgs, &arg[numArgs - 1]);
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

static int verifyArguments(Operation *op, CommandTokens *commandTokens) {
    /* make sure we have the correct amount of arguments */
    if (commandTokens->numArgs != op->numArgs) {
        ERROR_RET((_, "Wrong number of arguments for %s. Expected: %d Got: %d",
                op->name, op->numArgs, commandTokens->numArgs));
    }
    /* TODO: flatten `if`s? */
    /* */
    if (op->numArgs > 0) {
        if (op->srcAddressing[0] == AT_UNSET) {
            /* if the command has no src arg, then the first (and only) arg is dest */
            if (matchesAddressing(op->destAddressing, commandTokens->arg1, &commandTokens->arg1Data) != 0) {
                return -1;
            }
        } else {
            /* if the command has a src arg, then the first arg is src */
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

static int matchesAddressing(int validAddressingArr[5], char *arg, Argument *argData) {
    int i;
    if (findArgumentAddressingType(arg, argData) == -1) {
        return -1;
    }
    for (i = 0; validAddressingArr[i] != -1; i++) {
        if (validAddressingArr[i] == argData->addressing) {
            return 0;
        }
    }
    ERROR_RET((_, "Invalid addressing type: %s (%s)", addressingTypeStr(argData->addressing), arg));
}

/*----------------------
 *  Directives
 */
int handleDirective(DissectedDirective dissectedDirective) {
    /* In entry, extern we reference a symbol - validate the symbol */
    if ((dissectedDirective.type == DT_ENTRY) || (dissectedDirective.type == DT_EXTERN)) {
        if (validateLabel(dissectedDirective.directiveArgs) != 0) {
            return -1;
        }
    }

    if (dissectedDirective.type == DT_ENTRY) {
        return 0;
    }
    if (dissectedDirective.type == DT_EXTERN) {
        addSymbol(dissectedDirective.directiveArgs, 0, ST_EXTERNAL, FALSE);
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
            /*Check that the number we got can fit into 24 bits (the size of a "word")*/
            if ((number > MAX_24_BIT_WORD) || (number < MIN_24_BIT_WORD)) {
                ERROR_RET((_, "Number %ld out of range", number))
            }
            if (iterator == iteratorBuf) {
                ERROR_RET((_, "Entries in \".data\" must be comma separated integer numbers. Got <%s>", stripped));
            }
            addDataWord(number);
            stripWhiteSpaces(iterator, stripped);
            if (stripped[0] == ',') {
                stripped++;
                if (*stripped == 0) {
                    ERROR_RET((_, "Missing data after comma."));
                }
                continue;
            } else if (stripped[0] == 0) {
                /* finished data parsing */
                return 0;
            } else {
                /* error - expected end or ','. */
                ERROR_RET((_, "Expected ',', got unexpected characters: <%s>", stripped))
            }
        }
        /* TODO: empty .data : warning? */
        return 0;
    }
    /* invalid DirectiveType */
    ERROR_RET((_, "Unexpected value: "));
}
