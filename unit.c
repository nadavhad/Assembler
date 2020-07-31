#include "symbolTable.h"
#include "errorlog.h"
#include <stdio.h>


int main() {
    SymbolData symData;
    addSymbol("MAIN", 3, LT_COMMAND, FALSE);
    if (lookUp("MAIN", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }

    if (addSymbol("LOOP", 98, LT_COMMAND, FALSE) != 0) {
        printf("Error: Failed adding symbol\n");
        flush();
    }
    if (lookUp("LOOP", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }


    return 0;
}

