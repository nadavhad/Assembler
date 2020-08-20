#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H

#include "constants.h"

/**
 * An enum for symbol properties:
 * Code (commands),
 * Data (data/string directives),
 * External (external symbols)
 */
typedef enum {
    ST_CODE,
    ST_DATA,
    ST_EXTERNAL
} SymbolType;

enum bool {
    FALSE, TRUE
};

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

int startSymbolTableIteration(void **iterator, SymbolData *data);

int getSymbolTableNext(void **iterator, SymbolData *data);

#endif /*ASSEMBLER_SYMBOL_TABLE_H*/
