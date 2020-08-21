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
    /* open the file and make sure we succeeded to */
    file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open input file: ");
        return -1;
    }
    /* reinitialize file-iteration state */
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
            case LT_COMMENT:
                /* ignore empty line or comment */
                break;
            case LT_COMMAND:
                /* handle commands */
                handleCommand(dissectedLine);
                break;
            case LT_DIRECTIVE:
                /* figure out what directive this is */
                if (getDirectiveType(dissectedLine, &dissectedDirective) == -1) {
                    break;
                }
                /* if it's not an entry, we handled it in the first pass */
                if (dissectedDirective.type != DT_ENTRY) {
                    break;
                }
                /* mark that the referenced symbol is an entry */
                if (setEntrySymbol(dissectedDirective.directiveArgs) == -1) {
                    break;
                }
                break;
        }
    }
}
