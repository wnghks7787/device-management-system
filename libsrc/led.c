#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <softPwm.h>

#define LED 29

#define LED_100 255
#define LED_75 191
#define LED_50 127
#define LED_25 63
#define LED_0 0

void led_control(char* mode)
{
	if(strcmp(mode, "11") == 0)
	{
		softPwmWrite(LED, LED_100);
	}
	else if(strcmp(mode, "12") == 0)
	{
		softPwmWrite(LED, LED_75);
	}
	else if(strcmp(mode, "13") == 0)
	{
		softPwmWrite(LED, LED_50);
	}
	else if(strcmp(mode, "14") == 0)
	{
		softPwmWrite(LED, LED_25);
	}
	else
	{
		softPwmWrite(LED, LED_0);
	}
}
