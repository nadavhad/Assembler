SRCS = logging/errorlog.c assembler/assembler.c assembler/state.c assembler/dissector.c assembler/symbolTable.c assembler/externusage.c
HDRS = assembler/macros.h assembler/structs.h assembler/assembler.h assembler/constants.h assembler/parsing.h assembler/state.h assembler/symbolTable.h assembler/externusage.h
INCLUDE_PATH =  -Ilogging -Iassembler

out/assembler.out: ${SRCS} ${HDRS}
	mkdir -p out
	gcc -g  -Wall -ansi -pedantic ${SRCS} assembler/main.c ${INCLUDE_PATH} -o out/assembler.out

	#TODO delete this part from the makefile
out/unit.out: unit.c ${SRCS} ${HDRS}
	mkdir -p out
	gcc -g unit.c ${SRCS} -o out/unit.out ${INCLUDE_PATH} -Wall -ansi -pedantic

clean:
	rm -rf out/*

check: out/assembler.out
	./out/assembler.out test

$(V).SILENT:
