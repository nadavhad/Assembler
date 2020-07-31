assembler.out: out/errorlog.o out/assembler.o out/state.o out/dissector.o
	mkdir -p out
	gcc -g  -Wall -ansi -pedantic out/errorlog.o out/assembler.o out/state.o out/dissector.o -o out/assembler.out

out/errorlog.o: logging/errorlog.c logging/errorlog.h
	mkdir -p out
	gcc -c logging/errorlog.c -o out/errorlog.o -Wall -ansi -pedantic

out/assembler.o: assembler/assembler.c assembler/assembler.h out/errorlog.o
	mkdir -p out
	gcc -c assembler/assembler.c -o out/assembler.o -Ilogging -Iassembler -Wall -ansi -pedantic

out/dissector.o: assembler/dissector.c
	mkdir -p out
	gcc -c assembler/dissector.c -o out/dissector.o -Ilogging -Iassembler -Wall -ansi -pedantic

out/state.o: assembler/state.c assembler/structs.h assembler/state.h
	mkdir -p out
	gcc -c assembler/state.c -o out/state.o -Ilogging -Wall -ansi -pedantic


clean:
	rm -rf out/*

$(V).SILENT:
