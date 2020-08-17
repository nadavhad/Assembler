#include <stdio.h>
#include <string.h>
#include "outfile.h"
#include "externusage.h"
#include "state.h"
#include "symbolTable.h"
#include <errorlog.h>

static int writeLine(FILE *, int, long);

int createCodeOutputFile(char *basefilename) {
    int dc, ic;
    long word;
    FILE *outfile;
    char icfStr[8];
    strcat(basefilename, ".ob");
    outfile = fopen(basefilename, "w+");
    if (outfile == NULL) {
        ERROR_RET((_, "Failed to create .ob output file."))
    }
    sprintf(icfStr, "%d", getState()->ICF);
    fprintf(outfile, "%7s %d", icfStr, getState()->DCF);

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

static int writeLine(FILE *outfile, int num, long data) {
    fprintf(outfile, "%07d %06x\n", num, data);
    return 0;
}

int createEntryOutputFile(char *fileName) {
    FILE *file;
    void *iterator;
    SymbolData symbolData;
    char buf[MAX_FILE_NAME];
    startSymbolTableIteration(&iterator, &symbolData);
    if (iterator == NULL) {
        return 0;
    }
    strcpy(buf, fileName);
    strcat(buf, ".ent");
    file = fopen(buf, "w+");
    if (file == NULL) {
        ERROR_RET((_, "could not create .ent output file"))
    }
    while (iterator != NULL) {
        if (symbolData.isEntry == TRUE) {
            int address = symbolData.value;
            if (symbolData.type==ST_DATA) {
                address += getState()->ICF;
            }
            fprintf(file, "%s %07d\n", symbolData.name, address);
        }
        getSymbolTableNext(&iterator, &symbolData);
    }
    fclose(file);
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
    strcpy(buf, fileName);
    strcat(buf, ".ext");
    file = fopen(buf, "w+");
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