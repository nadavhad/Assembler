#ifndef ASSEMBLER_FIRSTPASS_H
#define ASSEMBLER_FIRSTPASS_H

#include "dissector.h"

/**
 * This function performs the first pass on the given file.
 * 1. Opens the file given as a parameter.
 * 2. Reads the file (line by line)
 * 3. dissects each line and acts accordingly.
 * @return 0 on success, -1 on failure.
 * */
int firstPass(char *fileName);

/**
 * Handles the command labels - adds the label to the symbol table as code
 * */
int handleCmdLabelFirstPass(DissectedLine dissectedLine);

/**
 * Handles the directive labels.
 * A label before .entry or .extern is ignored and a warning is printed.
 * An entry or extern label - is ignored in the first pass.
 * A data or string label - is added to the symbol table as data.
 * */
int handleDirectiveLabelFirstPass(DissectedLine line, DissectedDirective dissectedDirective);

#endif /*ASSEMBLER_FIRSTPASS_H*/
