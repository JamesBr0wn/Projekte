CFLAGS	= -Wall -pthread -lrt

um.exe: um.o
	gcc -Wall -pthread -o um.exe um.o -lpigpio -lrt

um.o: um.c
	gcc -Wall -pthread -c um.c

clean:
	rm um.exe um.o

run: um.exe
	sudo ./um.exe
