#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../logging/errorlog.h"
#include "outfile.h"
#include "state.h"

static int strchrreplace(char *str, char replaced, char replacement);

static int writeLine(FILE *, int, int);

int createCodeOutputFile(char *basefilename) {
    int dc = 0;
    int ic = 100;
    int word;
    FILE *outfile;
    char formattedICF[8];
    strcat(basefilename, ".obj");
    outfile = fopen(basefilename, "w+");
    if (outfile == NULL) {
        ERROR_RET((_, "Failed to create output file."))
    }
    sprintf(formattedICF, "%07d", getState()->ICF);
    strchrreplace(formattedICF, '0', ' ');
    fprintf(outfile, "%s %d", formattedICF, getState()->DCF);

    /* Print output file */
    for (ic = 100; ic < getState()->ICF; ++ic) {
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic * 3], 3);
        writeLine(outfile, ic, word);
    }
    for (dc = 0; dc < getState()->DCF; ++dc) {
        word = 0;
        memcpy(&word, &getState()->dataByteCode[dc * 3], 3);
        writeLine(outfile, dc + getState()->ICF, word);
    }
    fclose(outfile);
    return 0;
}

static int strchrreplace(char *str, char replaced, char replacement) {
    char *iter = strchr(str, replaced);
    while (iter != NULL) {
        *iter = replacement;
        iter = strchr(str, replaced);
    }
    return 0;
}

static int writeLine(FILE *outfile, int num, int data) {
    fprintf(outfile, "%07d %06x\n", num, data);
    return 0;
}