#include <string.h>
#include "../logging/errorlog.h"
#include "assembler.h"
#include "symbolTable.h"
#include "externusage.h"

int main(int argc, char **argv) {
    int i = 0;
    char buf[80];
    for (i = 1; i < argc; ++i) {
        strcpy(buf, argv[i]);
        strcat(buf,".as");
        processAssemblyFile(argv[i]);
        clearExternUsagesTable();
        clearSymbolTable();
        clearErrorLog();
    }

    return 0;
}