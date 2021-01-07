#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pigpio.h>

int main(int argc, char *argv[])
{
	printf("Start...\n");
	unsigned int uhwrev;
	uhwrev = gpioHardwareRevision();
	printf("gpioHardwareRevision %u unsigned, hx %x\n", uhwrev, uhwrev);

  unsigned int gpioVer;
  gpioVer = gpioVersion();
	printf("gpioVersion = %u\n", gpioVer);


	/* my PI returns 0xc03130 as revision from cat /proc/cpuinfo
	unsigned int ui = 0xc03130;
	printf("tst %u unsigned\n", ui);
	*/
	
	int status = gpioInitialise();
	printf("initialise says: %d\n", status);
	if (status < 0)
	{
		exit(1);
	}

	// --------------------------
	int ledPin1 = 4;
	// int ledPin2 = 17;
	int initLed1 = gpioSetMode(ledPin1, PI_OUTPUT);
	printf("set output1 to GPIO %d: status: %d (0 means ok)\n", ledPin1, initLed1);
	//int initLed2 = gpioSetMode(ledPin2, PI_OUTPUT);
	//printf("set output2 to GPIO %d: status: %d (0 means ok)\n", ledPin2, initLed2);
  int writeLed1 = gpioWrite(ledPin1, 1);
  printf("write 1 GPIO %d: status: %d (0 means ok)\n", ledPin1, writeLed1);
  
	sleep(1);
	// --------------------------
	gpioWrite(ledPin1, 0); // don't keep it on
	gpioTerminate();
}
