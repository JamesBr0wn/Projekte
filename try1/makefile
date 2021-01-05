CFLAGS	= -Wall -pthread -lrt

vers.exe: vers.o
	gcc $(CFLAGS) -o vers.exe -lpigpio vers.o

vers.o: vers.c
	gcc $(CFLAGS) -c vers.c

clean:
	rm vers.exe vers.o

run: vers.exe
	sudo ./vers.exe
