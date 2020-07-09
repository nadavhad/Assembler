out/errorlog.o: logging/errorlog.c logging/errorlog.h
	mkdir -p out
	gcc -c logging/errorlog.c -o out/errorlog.o -Wall -ansi -pedantic

out/assembler.o: assembler/assembler.c assembler/assembler.h
	mkdir -p out
	gcc -c assembler/assembler.c -o out/assembler.o -Wall -ansi -pedantic

assembler: out/errorlog.o out/assembler.o
	mkdir -p out
	gcc -g  -Wall -ansi -pedantic out/errorlog.o out/assembler.o -o out/assembler

clean:
	rm -rf out/*



$(V).SILENT: