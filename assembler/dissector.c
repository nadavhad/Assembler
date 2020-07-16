#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assembler.h"

#define EQ(c, n) c == n

#define EOS(c) EQ(c,'\0')
#define eqEOF(c) EQ(c, EOF)
#define EOL(c) EQ(c,'\n')

#define END(c) EOS(c) || eqEOF(c) || EOL(c)
#define WHT(c) EQ(c, ' ') || EQ(c, '\t')


int dissectLabel(char* rawLine, DissectedLine* dissectedLine) {
  char *accumulator;
  char *label = "";
  char *command = "";
  char *iterator = rawLine;
  int index = 0;
  enum LineType lineType = LT_COMMENT;
  /* strip leading whitespace */
  while (WHT(*iterator)) {
    iterator = iterator + 1;
  }
  if (EQ(*iterator, ';') || EOL(*iterator)) {
    strcpy(dissectedLine->command, "");
    strcpy(dissectedLine->label, "");
    dissectedLine->lineType = LT_COMMENT;
    return 0;
  }
  accumulator = malloc(sizeof(char[MAX_LINE_LENGTH]));
  index = 0;
  while (!(END(*iterator))) {
    if (*iterator == ':') {
      strcpy(dissectedLine->label, accumulator);
      accumulator = malloc(sizeof(char[MAX_LINE_LENGTH]));
      index = 0;
    }
    accumulator[index] = *iterator;

    index++;
    iterator++;
  }
  strcpy(dissectedLine->command, accumulator);
  if(dissectedLine->command[0] == '.') {
    dissectedLine->lineType = LT_DIRECTIVE;
  }
  return 0;
}