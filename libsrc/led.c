#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <softPwm.h>

#define LED 29

void led_control(char* mode)
{
	softPwmCreate(LED, 0, 255);

	if(strcmp(mode, "11") == 0)
	{
		softPwmWrite(LED, 255);
	}
	else if(strcmp(mode, "12") == 0)
	{
		softPwmWrite(LED, 191);
	}
	else if(strcmp(mode, "13") == 0)
	{
		softPwmWrite(LED, 127);
	}
	else if(strcmp(mode, "14") == 0)
	{
		softPwmWrite(LED, 63);
	}
	else
	{
		softPwmWrite(LED, 0);
	}
}
