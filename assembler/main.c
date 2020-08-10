#include "assembler.h"
#include <string.h>

int main(int argc, char **argv) {
    int i = 0;
    char buf[80];
    for (i = 1; i < argc; ++i) {
        strcpy(buf, argv[i]);
        strcat(buf,".as");
        processAssemblyFile(argv[i]);
    }

    return 0;
}