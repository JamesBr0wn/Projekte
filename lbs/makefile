CFLAGS	= -Wall -pthread -lrt

lbs.exe: lbs.o
	gcc -Wall -pthread -o lbs.exe lbs.o -lpigpio -lrt

lbs.o: lbs.c
	gcc -Wall -pthread -c lbs.c

clean:
	rm lbs.exe lbs.o

run: lbs.exe
	sudo ./lbs.exe
