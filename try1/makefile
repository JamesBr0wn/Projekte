CFLAGS	= -Wall -pthread -lrt

vers.exe: vers.o
	gcc -Wall -pthread -o vers.exe vers.o -lpigpio -lrt

vers.o: vers.c
	gcc -Wall -c vers.c

clean:
	rm vers.exe vers.o

run: vers.exe
	sudo ./vers.exe
