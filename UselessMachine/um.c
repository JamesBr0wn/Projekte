#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pigpio.h>

// global variables
enum machineStatus {STATUS_BASE, STATUS_RUNNING };
struct machineState
{
  enum machineStatus state;
} currentMachineState;

const unsigned int led1Pin = 17;
const unsigned int button1Pin = 22;
const unsigned int servo1Pin = 23;
const int deltastep = 25;
const int servo1limitlow = 750;
const int servo1limithigh = 2350;
const unsigned int baseServo1Pos = 1500;

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
  int initLed1 = gpioSetMode(led1Pin, PI_OUTPUT);
  if (initLed1 < 0)
  {
    printf("set output1 to GPIO %d: status: %d (0 means ok)\n", led1Pin, initLed1);
    return -1;
  }
  int initButton1 = gpioSetMode(button1Pin, PI_INPUT);
  if (initLed1 < 0)
  {
    return -1;
    printf("set input1 to GPIO %d: status: %d (0 means ok)\n", button1Pin, initButton1);
  }
  return 0;
}

void doExit()
{
  gpioWrite(led1Pin, 0); // don't keep it on... if it was on
  gpioSetAlertFunc(button1Pin, NULL); // remove the listener... if it was set
  gpioServo(servo1Pin, 0); // means stop PWM
  gpioTerminate();
}

void button1AlertFunc(int gpio, int level, uint32_t tick)
{
  // printf("Alert: gpio %d, level %d, tick %u\n", gpio, level, tick);
  if ( (STATUS_BASE == currentMachineState.state) && (0 == level) )
  {
    currentMachineState.state = STATUS_RUNNING;
    printf("Setting machine state to RUNNING\n");
  }
}

int doSetEndActivity()
{
  int ret = 0;
  int writeLed1 = gpioWrite(led1Pin, 0);
  if (0 > writeLed1)
  {
    printf("Fail setting LED\n");
    ret--;
  }
  int servo1Status = gpioServo(servo1Pin, baseServo1Pos);
  if (0 > servo1Status)
  {
    printf("Fail setting Servo1\n");
    ret--;
  }
  currentMachineState.state = STATUS_BASE;
  usleep(1000000);
  // serv should be base position, so switch it off
  servo1Status = gpioServo(servo1Pin, 0);
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
  int button1AlertSet = gpioSetAlertFunc(button1Pin, &button1AlertFunc);
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
      int writeLed1 = gpioWrite(led1Pin, 1);
      if (0 > writeLed1)
      {
        printf("Fail setting LED\n");
      }
      // do activity
      int delta = deltastep;
      int pos = baseServo1Pos;
      for (int i=0; i<100; i++)
      {
        pos += delta;
        int servo1Status = gpioServo(servo1Pin, pos);
        if (0 > servo1Status)
        {
          printf("Fail setting Servo1\n");
        }
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
      // end activity
      doSetEndActivity();
      countRuns++;
    }
  }

  // -------------------------- cleanup
  doExit();
}
