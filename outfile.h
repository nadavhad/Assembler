#ifndef ASSEMBLER_OUTFILE_H
#define ASSEMBLER_OUTFILE_H

/**
* Creates the output .obj file
* @return -1 on error
*/
int createEntryOutputFile(char *fileName);

/**
 * Creates the output .ext file
 * @return -1 on error
 */
int createExternalOutputFile(char *fileName);

/**
* Creates the output .ent file
* @return -1 on error
*/
int createCodeOutputFile(char *fileName);

#endif /*ASSEMBLER_OUTFILE_H*/
