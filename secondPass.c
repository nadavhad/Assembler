#include <stdio.h>
#include "constants.h"
#include "state.h"
#include "dissector.h"
#include "secondPass.h"
#include "assembler.h"
#include "symbolTable.h"

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
                    break;
                }
                if (dissectedDirective.type != DT_ENTRY) {
                    break;
                }
                if (setEntrySymbol(dissectedDirective.directiveArgs) == -1) {
                    break;
                }
                break;
        }
    }
}
