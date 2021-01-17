#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pigpio.h>

unsigned int led1Pin = 17;   // 4 is ok too
unsigned int button1Pin = 22;

void doExit()
{
	gpioWrite(led1Pin, 0); // don't keep it on... if it was on
	gpioTerminate();
}

int main(int argc, char *argv[])
{
	// -------------------------- init Lib
	printf("Start...\n");
	unsigned int uhwrev = gpioHardwareRevision();
	printf("gpioHardwareRevision %u unsigned, hx %x\n", uhwrev, uhwrev);

	/* my PI returns 0xc03130 as revision from cat /proc/cpuinfo
	unsigned int ui = 0xc03130;
	*/
  unsigned int gpioVer = gpioVersion();
	printf("gpioVersion = %u\n", gpioVer);
	
	int libStatus = gpioInitialise();
	printf("initialise says: %d\n", libStatus);
	if (libStatus < 0)
	{
		exit(1);
	}

	// -------------------------- init Pins
	int initLed1 = gpioSetMode(led1Pin, PI_OUTPUT);
	printf("set output1 to GPIO %d: status: %d (0 means ok)\n", led1Pin, initLed1);
	if (initLed1 < 0) doExit();
	int initButton1 = gpioSetMode(button1Pin, PI_INPUT);
	printf("set input1 to GPIO %d: status: %d (0 means ok)\n", button1Pin, initButton1);
	if (initLed1 < 0) doExit();

	// -------------------------- work
  int writeLed1 = gpioWrite(led1Pin, 1);
  printf("write 1 GPIO %d: status: %d (0 means ok)\n", led1Pin, writeLed1);
  
  int button1Status;
  for (int i= 0; i< 100; i++)
  {
		button1Status = gpioRead(button1Pin);
		printf("step %d get input1 status: %d (>=0 means ok)\n", i, button1Status);
		usleep(200000); // in microseconds
	}

	// -------------------------- cleanup
	doExit();
}
