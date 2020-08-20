SRCS = assembler.c state.c firstPass.c secondPass.c parsing.c externUsage.c symbolTable.c outfile.c errorLog.c dissector.c
HDRS = assembler.h state.h firstPass.h secondPass.h parsing.h externusage.h symbolTable.h outfile.h errorLog.h structs.h constants.h

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