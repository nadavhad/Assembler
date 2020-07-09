#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

typedef struct {
    char *label;
    char *command;
} Line;
//typedef struct {
//  char* command;
//  char* arg1;
//  char* arg2;
//} Command;

/**
 * Splits a line of assembly code into a struct containing the label and command (with its args)
 * @return a struct containing the label and command (with its args)
 */
Line dissectLabel(char*);

///**
// * Splits a command string into a command and its arguments
// * @return
// */
//Command dissectCommand(Line);

#endif //ASSEMBLER_ASSEMBLER_H
