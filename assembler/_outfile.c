#include "_outfile.h"
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#include <errorlog.h>

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

