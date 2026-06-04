#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <softPwm.h>

#define CDS 27
#define LED 29

#define BRIGHT 1
#define DARK 2

int* cds_control(char* mode)
{
	int* ret = (int*)malloc(sizeof(int));
	if(strcmp(mode, "31") == 0)
	{
		if(digitalRead(CDS) == HIGH)	
		{
			*ret = BRIGHT;
		}
		else
		{
			*ret = DARK;
		}
		return ret;
	}
	else if(strcmp(mode, "32") == 0)
	{
		if(digitalRead(CDS) == HIGH)
		{
			*ret = BRIGHT;
			softPwmWrite(LED, 0);
		}
		else
		{
			*ret = DARK;
			softPwmWrite(LED, 255);
			delay(100);
		}
		return ret;
	}

	*ret = -1;
	return ret;
}
