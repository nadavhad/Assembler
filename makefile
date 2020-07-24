assembler.out: out/errorlog.o out/assembler.o
	mkdir -p out
	gcc -g  -Wall -ansi -pedantic out/errorlog.o out/assembler.o -o out/assembler.out

out/errorlog.o: logging/errorlog.c logging/errorlog.h
	mkdir -p out
	gcc -c logging/errorlog.c -o out/errorlog.o -Wall -ansi -pedantic

out/assembler.o: assembler/assembler.c assembler/assembler.h out/errorlog.o
	mkdir -p out
	gcc -c assembler/assembler.c -o out/assembler.o -Ilogging -Wall -ansi -pedantic



clean:
	rm -rf out/*



$(V).SILENT: