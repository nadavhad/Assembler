#include <string.h>
#include <stdio.h>
#include "errorLog.h"
#include "symbolTable.h"
#include "externUsage.h"
#include "firstPass.h"
#include "secondPass.h"
#include "outfile.h"

/**
 * Processes the .as file. if there are errors - print them and stop, else - create the output files.
 * */
static int processAssemblyFile(char *basefileName);

/* TODO */
int main(int argc, char **argv) {
    int i;
    /* For each file */
    for (i = 1; i < argc; ++i) {
        /* Process the file */
        processAssemblyFile(argv[i]);
        /* Clear all of the Lists for the next file */
        clearExternUsagesTable();
        clearSymbolTable();
        clearErrorLog();
    }

    return 0;
}

/** Process the assembly file:
 * 1. first pass - if there are errors print them and stop (or move to next file if exists)
 * 2. second pass - if there are errors print them and stop (or move to next file if exists)
 * 3.create the output files (.ob, .ent, .ext)
 * @return 0 on success, -1 on failure
 * */
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