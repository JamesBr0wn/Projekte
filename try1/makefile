CFLAGS	= -Wall -pthread -lrt

vers: vers.o
	gcc $(CFLAGS) -o vers -lpigpio vers.o

vers.o: vers.c
	gcc $(CFLAGS) -c vers.c

clean:
	rm vers vers.o

run: vers
	sudo ./vers
