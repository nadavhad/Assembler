#include <string.h>
#include "parsing.h"
/**
 * Strips leading and trailing whitespace from a string.
 * Copies the result to stripped
 * @param rawStr the raw string
 * @param stripped the address to copy the stripped string to. Must be Non-NULL.
 * @return 0 on success, -1 on failure.
 */
int stripWhiteSpaces(char *rawStr, char stripped[MAX_LINE_LENGTH]) {
    char *end;
    /* empty output space */
    memset(stripped, 0, MAX_LINE_LENGTH);
    /* advance start ptr until we reach a non-whitespace char */
    while (WHT(*rawStr) && !EOS(*rawStr) && !END(*rawStr)) {
        rawStr++;
    }
    strncpy(stripped, rawStr, MAX_LINE_LENGTH);
    end = stripped + strlen(stripped) - 1;
    /* advance from end, place NUL char instead of each whitespace */
    while (WHT(*end)) {
        *end = 0;
        end--;
    }
    return 0;
}