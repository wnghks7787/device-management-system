#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define LED 29

void led_on(char* onoff)
{
	wirintPiSetup();

	pinMode(LED, OUTPUT);

	if(strcmp(onoff, "11") == 0)
	{
		digitalWrite(LED, HIGH);
	}
	else
	{
		digitalWrite(LED, LOW);
	}
}
