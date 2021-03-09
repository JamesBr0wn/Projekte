#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pigpio.h>

unsigned int led1Pin = 17;   // 4 is ok too
unsigned int button1Pin = 22;
unsigned int servo1Pin = 25; // or 23
uint32_t lastTick = 0;

int doInitLib()
{
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
		return -1;
	}
  return 1;
}

int doInit()
{
	// -------------------------- init Pins
	int initLed1 = gpioSetMode(led1Pin, PI_OUTPUT);
	printf("set output1 to GPIO %d: status: %d (0 means ok)\n", led1Pin, initLed1);
	if (initLed1 < 0) return -1;
	int initButton1 = gpioSetMode(button1Pin, PI_INPUT);
	printf("set input1 to GPIO %d: status: %d (0 means ok)\n", button1Pin, initButton1);
	if (initLed1 < 0) return -1;
	return 1;
}

void doExit()
{
	gpioWrite(led1Pin, 0); // don't keep it on... if it was on
	gpioSetAlertFunc(button1Pin, NULL); // remove the listener... if it was set
  gpioServo(servo1Pin, 0);
	gpioTerminate();
}

void button1AlertFunc(int gpio, int level, uint32_t tick)
{
	if (tick - lastTick > 200) // non-ideal switch sending multiple events
	{
		printf("\nAccepted: gpio %d, level %d, tick %u\n", gpio, level, tick);
	}
	else
	{
		// printf("Ignored: gpio %d, level %d, tick %u\n", gpio, level, tick);
	}
  lastTick = tick;
}

int main(int argc, char *argv[])
{
 
	// -------------------------- init Lib
	printf("Start...\n");
  if (doInitLib() < 0)
  {
		exit(1);		
	}
  if (doInit() < 0)
  {
		doExit();
		exit(1);
	}

	// -------------------------- work
	int writeLed1 = gpioWrite(led1Pin, 1);
	printf("write 1 GPIO %d: status: %d (0 means ok)\n", led1Pin, writeLed1);

  /* // polling
	int button1Status;
	for (int i= 0; i< 100; i++)
	{
		button1Status = gpioRead(button1Pin);
		printf("step %d get input1 status: %d (>=0 means ok)\n", i, button1Status);
		usleep(200000); // in microseconds
	}
  */

  // eventing
  int button1AlertSet = gpioSetAlertFunc(button1Pin, &button1AlertFunc);
  printf("input1 set alert: %d (>=0 means ok)\n", button1AlertSet);
  
  int servo1Status;
  unsigned int pos = 1500;
  // start in the middle
  servo1Status = gpioServo(servo1Pin, pos);
  printf("servo1 set to middle: %d (>=0 means ok)\n", servo1Status);
  if (servo1Status < 0)
  {
		doExit();
	}
  sleep(2);

  const int deltastep = 15;
  int delta = deltastep;
  const int servo1limitlow = 750;
  const int servo1limithigh = 2350;
  for (int i=0; i<1000; i++)
  {
		pos += delta;
    servo1Status = gpioServo(servo1Pin, pos);
    printf("servo1 pos set, status = %d (>=0 means ok), pos = %u\n", servo1Status, pos);
    usleep(30000);
    if (pos >= servo1limithigh)
    {
			delta = -1 * deltastep;
		}
		if (pos <= servo1limitlow)
		{
			delta = deltastep;
		}
	}

	/*
  servo1Status = gpioServo(servo1Pin, 1500);
  printf("servo1 set status: %d (>=0 means ok)\n", servo1Status);
  sleep(5);
  servo1Status = gpioServo(servo1Pin, 1000);
  printf("servo1 set status: %d (>=0 means ok)\n", servo1Status);
  sleep(5);
  servo1Status = gpioServo(servo1Pin, 2000);
  printf("servo1 set status: %d (>=0 means ok)\n", servo1Status);
  sleep(5);
  servo1Status = gpioServo(servo1Pin, 1500);
  printf("servo1 set status: %d (>=0 means ok)\n", servo1Status);
  sleep(5);
  */

	// -------------------------- cleanup
	doExit();
}
