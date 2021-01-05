CFLAGS	= -Wall -pthread -lrt

vers.exe: vers.o
	gcc $(CFLAGS) -o vers.exe -lpigpio vers.o

vers.o: vers.c
	gcc $(CFLAGS) -c vers.c

clean:
	rm vers vers.o

run: vers
	sudo ./vers
