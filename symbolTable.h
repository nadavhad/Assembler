#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H

#include "constants.h"
#include "types.h"

/**
 * A struct with the data of a symbol
 * */
typedef struct {
    char name[MAX_LABEL_LENGTH];
    int value;
    SymbolType type;
    enum bool isEntry;
} SymbolData;

/**
 * Add a symbol to the symbol table
 */
int addSymbol(char name[MAX_LABEL_LENGTH], int value, SymbolType type, enum bool isEntry);

/**
 * Look up a symbol in the symbol table
 */
int lookUp(char name[MAX_LABEL_LENGTH], SymbolData *symbolData);

/**
 *
 */
int setEntrySymbol(char *label);

enum bool isSymbolTableComplete();

int incrementDataSymbolsOffset(int icf);

void clearSymbolTable();

int startSymbolTableIteration(void **iterator, SymbolData *data);

int getSymbolTableNext(void **iterator, SymbolData *data);

#endif /*ASSEMBLER_SYMBOL_TABLE_H*/
