#include <string.h>
#include <stdio.h>
#include "errorLog.h"
#include "symbolTable.h"
#include "externUsage.h"
#include "firstPass.h"
#include "secondPass.h"
#include "outfile.h"

static int processAssemblyFile(char *basefileName);

int main(int argc, char **argv) {
    int i;
    for (i = 1; i < argc; ++i) {
        processAssemblyFile(argv[i]);
        clearExternUsagesTable();
        clearSymbolTable();
        clearErrorLog();
    }

    return 0;
}

int processAssemblyFile(char *basefileName) {
    char assemblyFileName[80];
    strcpy(assemblyFileName, basefileName);
    strcat(assemblyFileName, ".as");

    /* if the first pass failed (regardless of input validity) , exit */
    if (firstPass(assemblyFileName) != 0) {
        fprintf(stderr, "Error: firstPass Failed\n");
        return -1;
    }
    /* if we had any input errors during first pass, print them and exit */
    if (numErrors() > 0) {
        flush();
        return -1;
    }
    /* if the second pass failed (regardless of input validity) , exit */
    if (secondPass(assemblyFileName) != 0) {
        fprintf(stderr, "Error: secondPass Failed\n");
        return -1;
    }
    /* if we had any input errors during second pass, print them and exit */
    if (numErrors() > 0) {
        flush();
        return -1;
    }

    /* Write output files */
    if (writeOutputFiles(basefileName) != 0) {
        flush();
        return -1;
    }
    return 0;
}