#include "symbolTable.h"
#include "errorlog.h"
#include <string.h>
#include <stdio.h>
#include "assembler.h"

int getDirectiveType(DissectedLine dissectedLine, DissectedDirective *directive);

int stripWhiteSpaces(char *rawStr, char stripped[MAX_LINE_LENGTH]);


void addSymbolTest();

void getDirectiveTypeTest();

void stripWhiteSpacesTest();

void handleDirectiveTest();

int toBinary(int n, char binary[24]);

void expect(char *testName, char *s1, char *s2);

int main() {
    char binary[32];
    int word;
    addSymbolTest();

    getDirectiveTypeTest();

    stripWhiteSpacesTest();

    handleDirectiveTest();

    {
        int ic = 100;
        if (firstPass("unitTest.as") == -1) {
            flush();
            return 0;
        }

        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[0]", binary, "00000000000010110110100000001100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[1]", binary, "00000000000000000000000000000001");/*TODO: will only work in second pass*/
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[2]", binary, "00000000001101000000000000000100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[3]", binary, "00000000000000000000000110000100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[4]", binary, "00000000000100010001111000000100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[5]", binary, "00000000000000000000000000000001");/*TODO: will only work in second pass*/
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[6]", binary, "00000000000101000001111000011100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[7]", binary, "00000000000000110110100000000100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[8]", binary, "00000000000000000000000000000001");/*TODO: will only work in second pass*/
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[9]", binary, "00000000000010110011110000010100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[10]", binary, "00000000001001000000100000010100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[11]", binary, "00000000000000000000000000000001");/*TODO: will only work in second pass*/
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[12]", binary, "00000000000001010000000000000100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[13]", binary, "00000000000000000000000000000001");/*TODO: will only work in second pass*/
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[12]", binary, "00000000111111111111111111010100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[14]", binary, "00000000001001000001000000010100");
        ic += 3;
        word = 0;
        memcpy(&word, &getState()->currentByteCode[ic], 3);
        toBinary(word, binary);
        expect("word[15]", binary, "00000000000000000000000000000100");/*TODO: will only work in second pass*/


    }
   return 0;
}

int toBinary(int n, char binary[32]) {
    int c, k;
    binary[0] = 0;
    for (c = 31; c >= 0; c--) {
        k = n >> c;

        if (k & 1)
            strcat(binary, "1");
        else
            strcat(binary, "0");
    }
    return 0;
}

void expect(char *testName, char *s1, char *s2) {
    if (strcmp(s1, s2) == 0) {
        printf("(%s) Succeeded: %s\n", testName, s2);
    } else {
        printf("(%s) Error:\n", testName);
        printf("   expected <%s>\n", s2);
        printf("   got      <%s>\n", s1);
    }
}

void stripWhiteSpacesTest() {
    char buf[MAX_LINE_LENGTH];
    stripWhiteSpaces("   POPOP   ", buf);
    expect("stripWhiteSpacesTest", buf, "POPOP");
    stripWhiteSpaces("  a bc   34, f", buf);
    expect("stripWhiteSpacesTest", buf, "a bc   34, f");
    stripWhiteSpaces("  5,g \n 43 98 ", buf);
    expect("stripWhiteSpacesTest", buf, "5,g \n 43 98");


}

void getDirectiveTypeTest() {
    {
        DissectedLine line1 = {"", ".entr", LT_DIRECTIVE};
        DissectedDirective directive;
        if (getDirectiveType(line1, &directive) == 0) {
            printf("directive type is %s (%d)\n", (directive.type == DT_UNDEFINED) ? "Correct" : "Incorrect",
                   directive.type);
        } else {
            flush();
        }

    }
    {
        DissectedLine line2 = {"LOOP", ".entry", LT_DIRECTIVE};
        DissectedDirective directive2;
        if (getDirectiveType(line2, &directive2) == 0) {
            printf("directive type is %s (%d)\n", (directive2.type == DT_ENTRY) ? "Correct" : "Incorrect",
                   directive2.type);
        } else {
            flush();
        }
    }
    {
        DissectedLine line3 = {"Lar", ".extern JKHK", LT_DIRECTIVE};
        DissectedDirective directive3;
        if (getDirectiveType(line3, &directive3) == 0) {
            printf("directive type is %s (%d)\n", (directive3.type == DT_EXTERN) ? "Correct" : "Incorrect",
                   directive3.type);
            expect("getDirectiveType", directive3.directiveArgs, "JKHK");
        } else {
            flush();
        }
    }
    {
        DissectedLine line4 = {"MAN", ".data 66, 7, 887", LT_DIRECTIVE};
        DissectedDirective directive4;
        if (getDirectiveType(line4, &directive4) == 0) {
            printf("directive type is %s (%d)\n", (directive4.type == DT_DATA) ? "Correct" : "Incorrect",
                   directive4.type);
            expect("getDirectiveType", directive4.directiveArgs, "66, 7, 887");
        } else {
            flush();
        }
    }
    {
        DissectedLine line5 = {"XOR", ".string 'hjkghk'", LT_DIRECTIVE};
        DissectedDirective directive5;
        if (getDirectiveType(line5, &directive5) == 0) {
            printf("directive type is %s (%d)\n", (directive5.type == DT_STRING) ? "Correct" : "Incorrect",
                   directive5.type);
        } else {
            flush();
        }
    }
}

void addSymbolTest() {
    SymbolData symData;
    addSymbol("GMAIN", 3, LT_COMMAND, FALSE);
    if (lookUp("GMAIN", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }

    if (addSymbol("GLOOP", 98, LT_COMMAND, FALSE) != 0) {
        printf("Error: Failed adding symbol\n");
        flush();
    }
    if (lookUp("GLOOP", &symData) != 0) {
        printf("Error: Symbol not found!\n");
        flush();
    }
}

void handleDirectiveTest() {
    {
        DissectedDirective directive0 = {DT_ENTRY, "", "ABC"};
        if (handleDirective(directive0) != 0) {
            printf("handleDirectiveTest#1 failed...\n");
        }

    }

    {
        DissectedDirective directive1 = {DT_DATA, "", "   1, -3\t, 4"};
        if (handleDirective(directive1) != 0) {
            printf("handleDirectiveTest#2 failed...\n");
        }
    }

    {
        DissectedDirective directive2 = {DT_STRING, "", "\" heeeeee\""};
        if (handleDirective(directive2) != 0) {
            printf("handleDirectiveTest#3 failed...\n");
        }
    }
}