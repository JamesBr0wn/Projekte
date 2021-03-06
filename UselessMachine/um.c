#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pigpio.h>

// global variables
enum machineStatus {STATUS_BASE, STATUS_RUNNING };
struct machineState
{
  enum machineStatus state;
  uint32_t lastTick;
} currentMachineState;

const unsigned int ledWorkPin = 17;
const unsigned int ledReadyPin = 5;
const unsigned int buttonStartWorkPin = 22;
const unsigned int servoHoodPin = 23;
const int deltastep = 25;
const int servoHoodlimitlow = 750;
const int servoHoodlimithigh = 2350;
const unsigned int baseServoHoodPos = 1500;

int doInitLib()
{
  /*
  unsigned int uhwrev = gpioHardwareRevision();
  printf("gpioHardwareRevision %u unsigned, hx %x\n", uhwrev, uhwrev);

  // my PI400 returns 0xc03130 as revision from cat /proc/cpuinfo
  unsigned int gpioVer = gpioVersion();
  printf("gpioVersion = %u\n", gpioVer);
  */

  int libStatus = gpioInitialise();
  if (libStatus < 0)
  {
    printf("initialise: %d\n", libStatus);
    return -1;
  }
  return 0;
}

int doInit()
{
  // -------------------------- init Pins
  int initLed1 = gpioSetMode(ledWorkPin, PI_OUTPUT);
  if (initLed1 < 0)
  {
    printf("set output1 to GPIO %d: status: %d (0 means ok)\n", ledWorkPin, initLed1);
    return -1;
  }
  int initLed2 = gpioSetMode(ledReadyPin, PI_OUTPUT);
  if (initLed2 < 0)
  {
    printf("set output2 to GPIO %d: status: %d (0 means ok)\n", ledReadyPin, initLed2);
    return -1;
  }
  int writeLed2 = gpioWrite(ledReadyPin, 1);
  if (0 > writeLed2)
  {
    printf("Fail setting LED\n");
  }
  int initButton1 = gpioSetMode(buttonStartWorkPin, PI_INPUT);
  if (initLed1 < 0)
  {
    return -1;
    printf("set input1 to GPIO %d: status: %d (0 means ok)\n", buttonStartWorkPin, initButton1);
  }
  // initialize machine state
  currentMachineState.state = STATUS_BASE;
  currentMachineState.lastTick = 0;
  return 0;
}

void doExit()
{
  gpioWrite(ledWorkPin, 0); // don't keep it on... if it was on
  gpioWrite(ledReadyPin, 0);
  gpioSetAlertFunc(buttonStartWorkPin, NULL); // remove the listener... if it was set
  gpioServo(servoHoodPin, 0); // means stop PWM
  gpioTerminate();
}

void button1AlertFunc(int gpio, int level, uint32_t tick)
{
  // printf("Alert: gpio %d, level %d, tick %u\n", gpio, level, tick);
  if (tick - currentMachineState.lastTick > 400) // non-ideal switch sending multiple events
	{
		printf("\nAccepted: gpio %d, level %d, tick %u\n", gpio, level, tick);
	}
	else
	{
		// printf("Ignored: gpio %d, level %d, tick %u\n", gpio, level, tick);
    return;
	}
  currentMachineState.lastTick = tick;

  // 
  if ( (STATUS_BASE == currentMachineState.state) && (0 == level) )
  {
    currentMachineState.state = STATUS_RUNNING;
    printf("Setting machine state to RUNNING\n");
    return;
  }
}

int doSetEndActivity()
{
  int ret = 0;
  int writeLed1 = gpioWrite(ledWorkPin, 0);
  if (0 > writeLed1)
  {
    printf("Fail setting LED\n");
    ret--;
  }
  int servo1Status = gpioServo(servoHoodPin, baseServoHoodPos);
  if (0 > servo1Status)
  {
    printf("Fail setting Servo1\n");
    ret--;
  }
  currentMachineState.state = STATUS_BASE;
  usleep(1000000);
  // serv should be in base position, so switch it off
  servo1Status = gpioServo(servoHoodPin, 0);
  if (0 > servo1Status)
  {
    printf("Fail setting Servo1\n");
    ret--;
  }
  return ret;
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
  // eventing
  int button1AlertSet = gpioSetAlertFunc(buttonStartWorkPin, &button1AlertFunc);
  if (0 > button1AlertSet)
  {
    printf("input1 set alert: %d (>=0 means ok)\n", button1AlertSet);
  }
  
  // start in base position
  doSetEndActivity();
  sleep(1);

  int countRuns = 0;
  while (countRuns < 5)
  {
    usleep(500000);
    if (STATUS_RUNNING == currentMachineState.state)
    {
      int writeLed1 = gpioWrite(ledWorkPin, 1);
      if (0 > writeLed1)
      {
        printf("Fail setting LED\n");
      }
      // do activity
      int delta = deltastep;
      int pos = baseServoHoodPos;
      for (int i=0; i<100; i++)
      {
        pos += delta;
        int servo1Status = gpioServo(servoHoodPin, pos);
        if (0 > servo1Status)
        {
          printf("Fail setting Servo1\n");
        }
        usleep(30000);
        if (pos >= servoHoodlimithigh)
        {
          delta = -1 * deltastep;
        }
        if (pos <= servoHoodlimitlow)
        {
          delta = deltastep;
        }
      }
      // end activity
      doSetEndActivity();
      countRuns++;
    }
  }

  // -------------------------- cleanup
  doExit();
}
