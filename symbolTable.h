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
 * Mark a symbol as an entry
 */
int setEntrySymbol(char *label);

/**
 * Checks if the we have all symbol table data, and data is accurate.
 */
enum bool isSymbolTableComplete();

/**
 * Offsets all data symbols a certain amount (ICF)
 */
int incrementDataSymbolsOffset(int icf);

/**
 * Empty the symbol recordings
 */
void clearSymbolTable();

/**
 * Expose the first Node for iteration purposes
 */
int startSymbolTableIteration(void **iterator, SymbolData *data);

/**
 * Expose the next Node for iteration purposes
 */
int getSymbolTableNext(void **iterator, SymbolData *data);

#endif /*ASSEMBLER_SYMBOL_TABLE_H*/
