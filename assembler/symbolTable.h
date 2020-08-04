#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H

#include "structs.h"

enum bool {TRUE, FALSE};

typedef struct {
    char name[MAX_LABEL_LENGTH];
    int value;
    SymbolType type;
    enum bool isEntry;
} SymbolData;


int addSymbol(char name[MAX_LABEL_LENGTH], int value, SymbolType type, enum bool isEntry);

int lookUp(char name[MAX_LABEL_LENGTH], SymbolData *symbolData);

int updateSymbol(char name[MAX_LABEL_LENGTH], int value);



#endif /*ASSEMBLER_SYMBOL_TABLE_H*/