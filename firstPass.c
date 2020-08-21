#include <stdio.h>
#include <string.h>
#include "firstPass.h"
#include "dissector.h"
#include "state.h"
#include "symbolTable.h"
#include "parsing.h"
#include "errorLog.h"
#include "assembler.h"


/**
 * This function performs the first pass on the given file.
 * 1. Opens the file given as a parameter.
 * 2. Reads the file (line by line)
 * 3. dissects each line and acts accordingly.
 * @return 0 on success, -1 on failure.
 * */
int firstPass(char *fileName) {
    FILE *file;
    char line[MAX_LINE_LENGTH + 5];
    DissectedLine dissectedLine;
    DissectedDirective dissectedDirective;
    /* Opening the file*/
    file = fopen(fileName, "r");
    /* if we can't open the file, no point in continuing */
    if (file == NULL) {
        perror(fileName);
        return -1;
    }
    /* Initializing state */
    initializeState();
    while (1) {
        /* Read a line */
        if (fgets(line, MAX_LINE_LENGTH + 5, file) == NULL) {
            /* We got to the end of a file: close file, log IC and DC values */
            fclose(file);
            getState()->ICF = getState()->IC;
            getState()->DCF = getState()->DC;
            incrementDataSymbolsOffset(getState()->ICF);
            return 0;
        }
        incLineNumber();
        if (strlen(line) > MAX_LINE_LENGTH) {
            /* Read everything up to the end of the line (or file) */
            while (!EOL(line[strlen(line) - 1]) && fgets(line, MAX_LINE_LENGTH + 5, file) != NULL);
            logError(getLineNumber(), "Line is too long");
            continue;
        }
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
                        {
                            handleDirective(dissectedDirective);
                        }
                    }
                    break;
                }
        }
    }
}

/**
 * Handles the command labels - adds the label to the symbol table as code (ST_CODE).
 * @return 0 on success, -1 on failure.
 * */
int handleCmdLabelFirstPass(DissectedLine dissectedLine) {
    if (addSymbol(dissectedLine.label, getState()->IC, ST_CODE, FALSE) != 0) {
        /* failed to add label to the symbol table*/
        return -1;
    }
    return 0;
}

/**
 * Handles the directive labels.
 * A label before .entry or .extern is ignored and a warning is printed.
 * An entry or extern label - is ignored in the first pass.
 * A data or string label - is added to the symbol table as data (ST_DATA).
 * @return 0 on success, -1 on failure.
 * */
int handleDirectiveLabelFirstPass(DissectedLine dissectedLine, DissectedDirective dissectedDirective) {
    enum bool isEntry = FALSE;
    /* Any label before .entry/.extern is ignored. We issue a warning to the user. */
    if ((dissectedDirective.type == DT_ENTRY) || (dissectedDirective.type == DT_EXTERN)) {
        if (strlen(dissectedLine.label) > 0) {
            printf("%d: Warning: There is a label before %s - it will be ignored\n", getLineNumber(),
                   (dissectedDirective.type == DT_ENTRY) ? ".entry" : ".extern");
        }
        return 0;
    }
    /* The directive is either .string or .data - add to symbol table */
    if (addSymbol(dissectedLine.label, getState()->DC, ST_DATA, isEntry) != 0) {
        return -1;
    }
    return 0;
}
