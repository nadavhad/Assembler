#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "dissector.h"
#include "parsing.h"
#include "state.h"
#include "errorLog.h"
#include "macros.h"

int findArgumentAddressingType(const char *raw_arg, Argument *argument) {
    char *endptr;
    argument->addressing = AT_UNSET;
    argument->reg = 0;
    argument->value.scalar = 0;
    if ((raw_arg[0] == 'r') && (strlen(raw_arg) == 2) && (raw_arg[1] >= '0') && (raw_arg[1] <= '7')) {
        argument->addressing = AT_REGISTER;
        argument->reg = raw_arg[1] - '0';
        return 0;
    } else if (raw_arg[0] == '#') {
        argument->value.scalar = strtol((raw_arg + 1), &endptr, 10);
        /*Check that the number we got can fit into 21 bits (the size of a "word" excluding A R E bits)*/
        if ((argument->value.scalar > MAX_21_BIT_WORD) || (argument->value.scalar < MIN_21_BIT_WORD)) {
            ERROR_RET((_, "Number %ld out of range", argument->value.scalar))
        }
        if ((endptr == (raw_arg + 1)) || (*endptr != 0)) {
            ERROR_RET((_, "Argument addressed with # must be a number, not: %s", raw_arg));
        }

        argument->addressing = AT_IMMEDIATE;
        return 0;
    } else if (raw_arg[0] == '&') {
        argument->addressing = AT_RELATIVE;
        strcpy(argument->value.symbol, &raw_arg[1]);
        if (validateLabel(argument->value.symbol) == -1) {
            return -1;
        }
        return 0;
    }

    if (validateLabel(raw_arg) == -1) {
        return -1;
    }
    argument->addressing = AT_DIRECT;
    strcpy(argument->value.symbol, raw_arg);

    return 0;
}

int dissectLabel(char *rawLine, DissectedLine *dissectedLine) {
    char accumulator[MAX_LINE_LENGTH];
    char *iterator = rawLine;
    int index;

    memset(dissectedLine->label, 0, sizeof(dissectedLine->label));
    /* strip leading whitespace */
    while (WHT(*iterator)) {
        iterator = iterator + 1;
    }
    if (EQ(*iterator, ';') || EOL(*iterator)) {
        /* if comment/empty, fill&exit.*/
        strcpy(dissectedLine->command, "");
        strcpy(dissectedLine->label, "");
        dissectedLine->lineType = LT_COMMENT;
        return 0;
    }
    index = 0;
    memset(accumulator, 0, sizeof(accumulator));
    while (!END(*iterator)) {
        /* accumulate until reaching end or colon, assign accumulator accordingly*/
        if ((dissectedLine->label[0] == 0) && (*iterator == ':')) {
            /* if has a label, fill label field with accumulator and reset accumulator */
            strcpy(dissectedLine->label, accumulator);
            if (validateLabel(dissectedLine->label) == -1) {
                return -1;
            }
            memset(accumulator, 0, sizeof(accumulator));
            index = 0;
            iterator++;
            while (WHT(*iterator)) {
                iterator = iterator + 1;
            }
        }
        accumulator[index] = *iterator;

        index++;
        iterator++;
    }
    stripWhiteSpaces(accumulator, dissectedLine->command);
    if ((strlen(dissectedLine->label) > 0) && strlen(dissectedLine->command) == 0) {
        ERROR_RET((_, "Label followed by an empty line is illegal"));
    }

    if (dissectedLine->command[0] == '.') {
        dissectedLine->lineType = LT_DIRECTIVE;
    } else {
        dissectedLine->lineType = LT_COMMAND;
    }
    return 0;
}

/**
 * A table with all operation/command data
 */
static Operation ops[NUM_OPERATIONS] = {
        /*
        op  fun  name   src addressing                                              dst addressing*/
        {0,  0, "mov",  {AT_IMMEDIATE, AT_DIRECT, AT_REGISTER, AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 2},
        {1,  0, "cmp",  {AT_IMMEDIATE, AT_DIRECT, AT_REGISTER, AT_UNSET, AT_UNSET}, {AT_IMMEDIATE, AT_DIRECT,   AT_REGISTER, AT_UNSET, AT_UNSET}, 2},
        {2,  1, "add",  {AT_IMMEDIATE, AT_DIRECT, AT_REGISTER, AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 2},
        {2,  2, "sub",  {AT_IMMEDIATE, AT_DIRECT, AT_REGISTER, AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 2},
        {4,  0, "lea",  {AT_DIRECT,    AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 2},
        {5,  1, "clr",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {5,  2, "not",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {5,  3, "inc",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {5,  4, "dec",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {9,  1, "jmp",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_RELATIVE, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {9,  2, "bne",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_RELATIVE, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {9,  3, "jsr",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_RELATIVE, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {12, 0, "red",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_DIRECT,    AT_REGISTER, AT_UNSET,    AT_UNSET, AT_UNSET}, 1},
        {13, 0, "prn",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_IMMEDIATE, AT_DIRECT,   AT_REGISTER, AT_UNSET, AT_UNSET}, 1},
        {14, 0, "rts",  {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_UNSET,     AT_UNSET,    AT_UNSET,    AT_UNSET, AT_UNSET}, 0},
        {15, 0, "stop", {AT_UNSET,     AT_UNSET,  AT_UNSET,    AT_UNSET, AT_UNSET}, {AT_UNSET,     AT_UNSET,    AT_UNSET,    AT_UNSET, AT_UNSET}, 0}
};
/**
 * Directive names/strings
 */
static char directives[4][7] = {
        "entry",
        "data",
        "extern",
        "string",
};

int validateLabel(const char *label) {
    int i;
    char *iter;
    /*
     * Requirements:
     * 1. label[0] is alphabetic
     * 2. len(label) <= 31
     * 3. unique
     * 4. not reserved:
     *  a. not r0..r7
     *  b. not command
     *  c. not directive
     * 5. All characters are alphanumeric
     */
    if (!(isalpha(label[0]))) ERROR_RET((_, "Illegal label, labels must start with a letter. Label: %s", label));

    if (strlen(label) > MAX_LABEL_LENGTH) ERROR_RET(
            (_, "Illegal label, max label length is 31 characters. Label: %s", label));

    if ((strlen(label) == 3)
        && (label[0] == 'r')
        && ((label[1] >= '0') && (label[1] <= '7'))) ERROR_RET((_,
            "Illegal label, label cannot be a register name. Label: %s", label));

    for (i = 0; i < NUM_OPERATIONS; i++) {
        if (strcmp(label, ops[i].name) == 0) ERROR_RET(
                (_, "Illegal label, label cannot be a command name. Label: %s", label));
    }

    for (i = 0; i < 5; i++) {
        if (strcmp(label, directives[i]) == 0) ERROR_RET(
                (_, "Illegal label, label cannot be a directive name. Label: %s",
                        label));
    }

    for (iter = (char *) label; *iter != 0; ++iter) {
        if (!isalnum(*iter)) {
            ERROR_RET((_, "Illegal label, labels must be alphanumeric: %s", label));
        }
    }
    return 0;
}

int findOperation(char *cmd, Operation *op) {
    int i;
    for (i = 0; i < NUM_OPERATIONS; i++) {
        if (strcmp(cmd, ops[i].name) == 0) {
            (*op) = ops[i];
            return 0;
        }
    }
    /* if we didn't return until here, we didn't find the command in the operation table */
    ERROR_RET((_, "Undefined operation: %s", cmd));
}
