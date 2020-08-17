#include <string.h>
#include "../logging/errorlog.h"
#include "assembler.h"
#include "symbolTable.h"
#include "externusage.h"

int main(int argc, char **argv) {
    int i = 0;
    for (i = 1; i < argc; ++i) {
        processAssemblyFile(argv[i]);
        clearExternUsagesTable();
        clearSymbolTable();
        clearErrorLog();
    }

    return 0;
}