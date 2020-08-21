#include <stdio.h>
#include <string.h>
#include "outfile.h"
#include "externUsage.h"
#include "state.h"
#include "symbolTable.h"
#include "errorLog.h"

/**
* Creates the output .obj file
* @return -1 on error
*/
static int createEntryOutputFile(char *fileName);

/**
 * Creates the output .ext file
 * @return -1 on error
 */
static int createExternalOutputFile(char *fileName);

/**
* Creates the output .ent file
* @return -1 on error
*/
static int createCodeOutputFile(char *fileName);

/**
 * Writes a line to the file with the address (IC/DC) and hexadecimal value
 */
static int writeLine(FILE *outfile, int address, long data);

/**
 * Opens a new file
 * @param basefilename the suffix-less filename
 * @param extension the file suffix/type
 * @param outFileName [ptr return value] the full filename
 * @return the file
 */
static FILE *openOutFile(const char *basefilename, const char *extension, char *outFileName);

static int writeLine(FILE *outfile, int address, long data) {
    fprintf(outfile, "%07d %06lx\n", address, data);
    return 0;
}

static FILE *openOutFile(const char *basefilename, const char *extension, char *outFileName) {
    FILE *outfile;
    /* concat the suffix to the filename */
    strcpy(outFileName, basefilename);
    strcat(outFileName, extension);
    /* open the file */
    outfile = fopen(outFileName, "w+");
    return outfile;
}

static int createCodeOutputFile(char *basefilename) {
    int dc, ic;
    long word;
    FILE *outfile;
    char icfStr[8];
    char outFileName[MAX_FILE_NAME];
    /* open the file */
    outfile = openOutFile(basefilename, ".ob", outFileName);
    /* make sure we were able to create a file */
    if (outfile == NULL) {
        ERROR_RET((_, "Failed to create .ob output file."))
    }
    /* format IC DC header line */
    sprintf(icfStr, "%d", getState()->ICF - 100);
    fprintf(outfile, "%7s %d\n", icfStr, getState()->DCF);

    /* Print output file */
    for (ic = 100; ic < getState()->ICF; ++ic) {
        word = 0;
        /* print each line of command data */
        memcpy(&word, &getState()->currentByteCode[ic * 3], 3);
        writeLine(outfile, ic, word);
    }
    for (dc = 0; dc < getState()->DCF; ++dc) {
        word = 0;
        /* print each line of directive data (".data"/".string") */
        memcpy(&word, &getState()->dataByteCode[dc * 3], 3);
        writeLine(outfile, dc + getState()->ICF, word);
    }
    fclose(outfile);
    return 0;
}

static int createEntryOutputFile(char *fileName) {
    FILE *file = NULL;
    void *iterator;
    SymbolData symbolData;
    char buf[MAX_FILE_NAME];
    /* get initial iteratior */
    startSymbolTableIteration(&iterator, &symbolData);
    /* if the list is empty, don't create a file */
    if (iterator == NULL) {
        return 0;
    }
    while (iterator != NULL) {
        if (symbolData.isEntry == TRUE) {
            int address = symbolData.value;
            if (file == NULL) {
                /* open the file if we didn't yet */
                file = openOutFile(fileName, ".ent", buf);
                if (file == NULL) {
                    ERROR_RET((_, "could not create .ent output file"))
                }
            }
            /* print entry symbols */
            fprintf(file, "%s %07d\n", symbolData.name, address);
        }
        /* advance iterator */
        getSymbolTableNext(&iterator, &symbolData);
    }
    if (file != NULL) {
        fclose(file);
    }
    return 0;
}

static int createExternalOutputFile(char *fileName) {
    FILE *file;
    void *iterator;
    ExternUsage externUsage;
    char buf[MAX_FILE_NAME];
    /* get initial iteratior */
    startExternUsageIteration(&iterator, &externUsage);
    /* if the list is empty, don't create a file */
    if (iterator == NULL) {
        return 0;
    }
    /* open file and make sure we got one */
    file = openOutFile(fileName, ".ext", buf);
    if (file == NULL) {
        ERROR_RET((_, "could not create .ext output file"))
    }
    while (iterator != NULL) {
        /* print extern symbols */
        fprintf(file, "%s %07d\n", externUsage.externLabel, externUsage.usageAddress);
        /* advance iterator */
        getExternUsageNext(&iterator, &externUsage);
    }
    fclose(file);
    return 0;
}

/*************************
 *  Write output files
 */
int writeOutputFiles(char *basefilename) {
    createCodeOutputFile(basefilename);
    createEntryOutputFile(basefilename);
    createExternalOutputFile(basefilename);
    return 0;
}