#include <stdio.h>

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
}
