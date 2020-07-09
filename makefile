errorlog.o:logging/errorlog.c logging/errorlog.h
	gcc -c logging/errorlog.c -o errorlog.o -Wall -ansi -pedantic

clean:
	rm -f *.out *.o



$(V).SILENT: