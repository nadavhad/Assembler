#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing.h"
#include "../logging/errorlog.h"
#include <ctype.h>

#define NUM_OPERATIONS 17
#define EQ(c, n) c == n
#define EOS(c) EQ(c,'\0')
#define eqEOF(c) EQ(c, EOF)
#define EOL(c) EQ(c,'\n')

#define END(c) EOS(c) || eqEOF(c) || EOL(c)
#define WHT(c) EQ(c, ' ') || EQ(c, '\t')


int findArgumentAddressingType(const char *arg) {
    int i;
    switch (arg[0]) {
        case 'r':
            if (('0' <= arg[1] && arg[1] <= '7') && (*(arg + 2) == '\0')) {
                return AT_REGISTER;
            } else {
                char *errormsg = "Attempted access to undefined register: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            }
        case '#':
            i = 1;
            while (isdigit(*(arg + i))) {
                i++;
            }
            if (*(arg + i) != '\0') {
                char *errormsg = "Argument addressed with # must be a number, not: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            } else return AT_IMMEDIATE;
        case '&':
            if (!requiresLabel(arg)) {
                char *errormsg = "Argument addressed with & must be a label, cannot find label: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            } else return AT_RELATIVE;
        default:
            return AT_DIRECT;
    }
}

int dissectLabel(char *rawLine, DissectedLine *dissectedLine) {
    char *accumulator;
    char *iterator = rawLine;
    int index = 0;
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
    accumulator = malloc(sizeof(char[MAX_LINE_LENGTH]));
    index = 0;
    while (!(END(*iterator))) {
        /* accumulate until reaching end or colon, assign accumulator accordingly*/
        if (*iterator == ':') {
            /* if has a label, fill label field with accumulator and reset accumulator */
            strcpy(dissectedLine->label, accumulator);
            accumulator = malloc(sizeof(char[MAX_LINE_LENGTH]));
            index = 0;
        }
        accumulator[index] = *iterator;

        index++;
        iterator++;
    }
    strcpy(dissectedLine->command, accumulator);
    if (dissectedLine->command[0] == '.') {
        dissectedLine->lineType = LT_DIRECTIVE;
        /* TODO: handle directives*/
    }
    return 0;
}

static Operation ops[NUM_OPERATIONS] = {
        /*op  fun  name   src addressing       dst addressing       */
        {0,  -1, "mov",  {0,  1,  3,  -1, -1}, {1,  3,  -1, -1, -1}},
        {1,  -1, "cmp",  {0,  1,  3,  -1, -1}, {0,  1,  3,  -1, -1}},
        {2,  1,  "add",  {0,  1,  3,  -1, -1}, {1,  3,  -1, -1, -1}},
        {2,  2,  "sub",  {0,  1,  3,  -1, -1}, {1,  3,  -1, -1, -1}},
        {4,  -1, "lea",  {1,  -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {5,  1,  "clr",  {-1, -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {5,  2,  "not",  {-1, -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {5,  3,  "inc",  {-1, -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {5,  4,  "dec",  {-1, -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {9,  1,  "jmp",  {-1, -1, -1, -1, -1}, {1,  2,  -1, -1, -1}},
        {9,  2,  "bne",  {-1, -1, -1, -1, -1}, {1,  2,  -1, -1, -1}},
        {9,  3,  "jsr",  {-1, -1, -1, -1, -1}, {1,  2,  -1, -1, -1}},
        {12, -1, "red",  {-1, -1, -1, -1, -1}, {1,  3,  -1, -1, -1}},
        {13, -1, "prn",  {-1, -1, -1, -1, -1}, {0,  1,  3,  -1, -1}},
        {14, -1, "rts",  {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}},
        {15, -1, "stop", {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}}
};

int findOperation(char *cmd, Operation *op) {
    int i;
    char *errormsg = "Undefined operation: ";
    for (i = 0; i < NUM_OPERATIONS; i++) {
        if (strcmp(cmd, ops[i].name) == 0) {
            (*op) = ops[i];
            return 0;
        }
    }
    strcat(errormsg, cmd);
    logError(-1, errormsg);
    return -1;
}