#include <stdio.h>
#include <string.h>
#include "outfile.h"
#include "externUsage.h"
#include "state.h"
#include "symbolTable.h"
#include "errorLog.h"

static int writeLine(FILE *, int, long);

FILE *openOutFile(const char *basefilename, const char *extension, char *outFileName);

int createCodeOutputFile(char *basefilename) {
    int dc, ic;
    long word;
    FILE *outfile;
    char icfStr[8];
    char outFileName[MAX_FILE_NAME];
    outfile = openOutFile(basefilename, ".ob", outFileName);
    if (outfile == NULL) {
        ERROR_RET((_, "Failed to create .ob output file."))
    }
    sprintf(icfStr, "%d", getState()->ICF - 100);
    fprintf(outfile, "%7s %d\n", icfStr, getState()->DCF);

    /* Print output file */
    for (ic = 100; ic < getState()->ICF; ++ic) {
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic * 3], 3);
        writeLine(outfile, ic, word);
    }
    for (dc = 0; dc < getState()->DCF; ++dc) {
        word = 0;
        memcpy(&word, &getState()->dataByteCode[dc * 3], 3);
        writeLine(outfile, dc + getState()->ICF, word);
    }
    fclose(outfile);
    return 0;
}

FILE *openOutFile(const char *basefilename, const char *extension, char *outFileName) {
    FILE *outfile;
    strcpy(outFileName, basefilename);
    strcat(outFileName, extension);
    outfile = fopen(outFileName, "w+");
    return outfile;
}

static int writeLine(FILE *outfile, int num, long data) {
    fprintf(outfile, "%07d %06lx\n", num, data);
    return 0;
}

int createEntryOutputFile(char *fileName) {
    FILE *file = NULL;
    void *iterator;
    SymbolData symbolData;
    char buf[MAX_FILE_NAME];
    startSymbolTableIteration(&iterator, &symbolData);
    if (iterator == NULL) {
        return 0;
    }
    while (iterator != NULL) {
        if (symbolData.isEntry == TRUE) {
            int address = symbolData.value;
            if (file == NULL) {
                file = openOutFile(fileName, ".ent", buf);
                if (file == NULL) {
                    ERROR_RET((_, "could not create .ent output file"))
                }
            }
            fprintf(file, "%s %07d\n", symbolData.name, address);
        }
        getSymbolTableNext(&iterator, &symbolData);
    }
    if (file != NULL) {
        fclose(file);
    }
    return 0;
}

int createExternalOutputFile(char *fileName) {
    FILE *file;
    void *iterator;
    ExternUsage externUsage;
    char buf[MAX_FILE_NAME];
    startExternUsageIteration(&iterator, &externUsage);
    if (iterator == NULL) {
        return 0;
    }
    file = openOutFile(fileName, ".ext", buf);
    if (file == NULL) {
        ERROR_RET((_, "could not create .ext output file"))
    }
    while (iterator != NULL) {
        fprintf(file, "%s %07d\n", externUsage.externLabel, externUsage.usageAddress);
        getExternUsageNext(&iterator, &externUsage);
    }
    fclose(file);
    return 0;
}