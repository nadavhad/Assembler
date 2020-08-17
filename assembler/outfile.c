#include <stdio.h>
#include <string.h>
#include "outfile.h"
#include "state.h"
#include "symbolTable.h"
#include <errorlog.h>

static int strchrreplace(char *str, char replaced, char replacement);

static int writeLine(FILE *, int, int);

int createCodeOutputFile(char *basefilename) {
    int dc = 0;
    int ic = 100;
    int word;
    FILE *outfile;
    char formattedICF[8];
    strcat(basefilename, ".obj");
    outfile = fopen(basefilename, "w+");
    if (outfile == NULL) {
        ERROR_RET((_, "Failed to create output file."))
    }
    sprintf(formattedICF, "%07d", getState()->ICF);
    strchrreplace(formattedICF, '0', ' ');
    fprintf(outfile, "%s %d", formattedICF, getState()->DCF);

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

static int strchrreplace(char *str, char replaced, char replacement) {
    char *iter = strchr(str, replaced);
    while (iter != NULL) {
        *iter = replacement;
        iter = strchr(str, replaced);
    }
    return 0;
}

static int writeLine(FILE *outfile, int num, int data) {
    fprintf(outfile, "%07d %06x\n", num, data);
    return 0;
}

int createEntryFile(char *fileName) {
    FILE* file;
    void *iterator;
    SymbolData symbolData;
    char buf[MAX_FILE_NAME];
    startSymbolTableIteration(&iterator, &symbolData);
    if(iterator != NULL){
        return 0;
    }
    strcpy(buf, fileName);
    strcat(buf,".ent");
    file = fopen(buf , "w+");
    if(file == NULL){
        ERROR_RET((_,"could not create output file"))
    }
    while (iterator != NULL) {
        if (symbolData.isEntry == TRUE) {
            int address = symbolData.value;
            if (symbolData.type==ST_DATA) {
                address += getState()->ICF;
            }
            fprintf(file, "%s %07d", symbolData.name, address);
        }
        getSymbolTableNext(&iterator, &symbolData);
    }
    fclose(file);
    return 0;
}