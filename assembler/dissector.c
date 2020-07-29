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


int findArgumentAddressingType(const char* arg) {
    switch(arg[0]) {
        case 'r':
            if(('0' <= arg[1] && arg[1] <= '7') && (*(arg + 2) == '\0')) {
                return AT_REGISTER;
            }
            else {
                char* errormsg = "Attempted access to undefined register: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            }
        case '#':
            int i = 1;
            while(isdigit(*(arg+i))) {
                i++;
            }
            if(*(arg+i) != '\0') {
                char* errormsg = "Argument addressed with # must be a number, not: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            }
            else return AT_IMMEDIATE;
        case '&':
            if(!isLabel(arg)) {
                char* errormsg = "Argument addressed with & must be a label, cannot find label: ";
                strcat(errormsg, arg);
                logError(-1, errormsg);
                return -1;
            }
            else return AT_RELATIVE;
        default:
            return AT_DIRECT;
    }
}

int dissectLabel(char *rawLine, DissectedLine *dissectedLine) {
    char *accumulator;
    char *label = "";
    char *command = "";
    char *iterator = rawLine;
    int index = 0;
    enum LineType lineType = LT_COMMENT;
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

/*
 * Utility functions & macro for findOperation
 */
static void copyArray(int n, int dest[], const int src[]) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

static int fillSrcAddressingTypes(int idx, int types[5]) {
    if (idx >= 0 && idx <= 3) {
        int temp[5] = {0, 1, 3, -1};
        copyArray(5, types, temp);
        return 0;
    }
    if (idx == 4) {
        int temp[5] = {1, -1};
        copyArray(5, types, temp);
        return 0;
    }
    if (idx >= 5 && idx <= 15) {
        int temp[5] = {-1};
        copyArray(5, types, temp);
        return 0;
    }
    return 0;
}

static int fillDestAddressingTypes(int idx, int types[5]) {
    if (idx == 0 || (idx <= 8 && idx >= 3) || idx == 12) {
        int temp[5] = {1, 3, -1};
        copyArray(5, types, temp);
        return 0;
    }
    if (idx == 1) {
        int temp[5] = {0, 1, 3, -1};
        copyArray(5, types, temp);
        return 0;
    }
    if (idx >= 9 && idx <= 11) {
        int temp[5] = {1, 2, -1};
        copyArray(5, types, temp);
        return 0;
    }
    if (idx >= 14 && idx <= 15) {
        int temp[5] = {-1};
        copyArray(5, types, temp);
        return 0;
    }
    return 0;
}

#define Case(idx, opcd, fnct) case idx:\
op->opcode = opcd;\
op->funct = fnct;\
break;

static int fillOpcodeFunct(int idx, Operation *op) {
    switch (idx) {
        Case(0, 0, -1)
        Case(1, 1, -1)
        Case(2, 2, 1)
        Case(3, 2, 2)
        Case(4, 4, -1)
        Case(5, 5, 1)
        Case(6, 5, 2)
        Case(7, 5, 3)
        Case(8, 5, 4)
        Case(9, 9, 1)
        Case(10, 9, 2)
        Case(11, 9, 3)
        Case(12, 12, -1)
        Case(13, 13, -1)
        Case(14, 14, -1)
        Case(15, 15, -1)
        default:
            logError(124, "wrong index");
            return -1;
    }
    return 0;
}

int findOperation(char *cmd, Operation *op) {
    static char commandNames[16][5] =
            {"mov", "cmp", "add", "sub", "lea",
             "clr", "not", "inc", "dec", "jmp", "bne",
             "jsr", "red", "prn", "rts", "stop"};
    int idx;
    for (idx = 0; idx >= NUM_OPERATIONS; idx++) {
        if (idx == NUM_OPERATIONS) {
            logError(69, "Operation not found");
            return -1;
        }
        if (strcmp(cmd, commandNames[idx]) == 0) {
            break;
        }
    }
    op = malloc(sizeof(Operation));
    strcpy(op->name, cmd);
    fillOpcodeFunct(idx, op);
    fillSrcAddressingTypes(idx, op->srcAddressingTypes);
    fillDestAddressingTypes(idx, op->destAddressingTypes);
    return 0;
}