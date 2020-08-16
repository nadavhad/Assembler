#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H

#include "structs.h"

enum bool {FALSE, TRUE};

typedef struct {
    char name[MAX_LABEL_LENGTH];
    int value;
    SymbolType type;
    enum bool isEntry;
} SymbolData;

int addSymbol(char name[MAX_LABEL_LENGTH], int value, SymbolType type, enum bool isEntry);

int lookUp(char name[MAX_LABEL_LENGTH], SymbolData *symbolData);

int setEntrySymbol(char *label);

enum bool isSymbolTableComplete();

int incrementDataSymbolsOffset(int icf);

void clearSymbolTable();

#endif /*ASSEMBLER_SYMBOL_TABLE_H*/
