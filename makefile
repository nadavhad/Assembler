SRCS = errorlog.c assembler.c state.c dissector.c symbolTable.c externusage.c outfile.c
HDRS = macros.h structs.h assembler.h constants.h parsing.h state.h symbolTable.h externusage.h outfile.h

assembler: ${SRCS} ${HDRS}
	gcc -g  -Wall -ansi -pedantic ${SRCS} main.c -o assembler

clean:
	rm assembler unit tests/*.ob tests/*.ent tests/*.ext

check: assembler
	./assembler tests/test

$(V).SILENT:

	#TODO delete this part from the makefile
unit: unit.c ${SRCS} ${HDRS}
	gcc -g unit.c ${SRCS} -o unit -Wall -ansi -pedantic