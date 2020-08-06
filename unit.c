#include "symbolTable.h"
#include "errorlog.h"
#include <stdio.h>
#include <assembler.h>

int getDirectiveType(DissectedLine dissectedLine, DissectedDirective *directive);



int main() {
    SymbolData symData;
    addSymbol("MAIN", 3, LT_COMMAND, FALSE);
    if (lookUp("MAIN", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }

    if (addSymbol("LOOP", 98, LT_COMMAND, FALSE) != 0) {
        printf("Error: Failed adding symbol\n");
        flush();
    }
    if (lookUp("LOOP", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }

    {
        DissectedLine line1 = {"", ".entr", LT_DIRECTIVE};
        DissectedDirective directive;
        if (getDirectiveType(line1, &directive) == 0) {
            printf("directive type is %s (%d)\n", (directive.type == DT_UNDEFINED)? "Correct":"Incorrect", directive.type);
        } else {
            flush();
        }

    }
    {
        DissectedLine line2 = {"LOOP", ".entry", LT_DIRECTIVE};
        DissectedDirective directive2;
        if (getDirectiveType(line2, &directive2) == 0) {
            printf("directive type is %s (%d)\n", (directive2.type == DT_ENTRY)? "Correct":"Incorrect", directive2.type);
        } else {
            flush();
        }

    }
    {
        DissectedLine line3 = {"Lar", ".extern", LT_DIRECTIVE};
        DissectedDirective directive3;
        if (getDirectiveType(line3, &directive3) == 0) {
            printf("directive type is %s (%d)\n", (directive3.type == DT_EXTERN) ? "Correct" : "Incorrect",
                   directive3.type);
        } else {
            flush();
        }
    }
    {
        DissectedLine line4 = {"MAN", ".data", LT_DIRECTIVE};
        DissectedDirective directive4;
        if (getDirectiveType(line4, &directive4) == 0) {
            printf("directive type is %s (%d)\n", (directive4.type == DT_DATA) ? "Correct" : "Incorrect",
                   directive4.type);
        } else {
            flush();
        }
    }
    {
        DissectedLine line5 = {"XOR", ".string", LT_DIRECTIVE};
        DissectedDirective directive5;
        if (getDirectiveType(line5, &directive5) == 0) {
            printf("directive type is %s (%d)\n", (directive5.type == DT_STRING) ? "Correct" : "Incorrect",
                   directive5.type);
        } else {
            flush();
        }
    }
    return 0;
}

