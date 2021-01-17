CFLAGS	= -Wall -pthread -lrt

lb.exe: lb.o
	gcc -Wall -pthread -o lb.exe lb.o -lpigpio -lrt

lb.o: lb.c
	gcc -Wall -pthread -c lb.c

clean:
	rm lb.exe lb.o

run: lb.exe
	sudo ./lb.exe
