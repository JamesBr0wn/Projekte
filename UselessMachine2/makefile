CFLAGS	= -Wall -pthread -lrt

um2.exe: um2.o
	gcc -Wall -pthread -o um2.exe um2.o -lpigpio -lrt

um2.o: um2.c
	gcc -Wall -pthread -c um2.c

clean:
	rm um2.exe um2.o

run: um2.exe
	sudo ./um2.exe
