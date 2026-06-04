#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define A 1
#define B 3
#define C 2
#define D 0

#define BUZZER 28

void fnd_control(char* mode)
{
	int start;
	int gpioins[4] = {D, C, B, A};

	int number[10][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 1},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 1, 0, 0},
		{0, 1, 0, 1},
		{0, 1, 1, 0},
		{0, 1, 1, 1},
		{1, 0, 0, 0},
		{1, 0, 0, 1}
	};

	start = mode[1] - '0';

	for(int i = 0 ; i < 4 ; i++)
	{
		pinMode(gpioins[4], OUTPUT);
	}

	for(int i = start ; i >= 0 ; i--)
	{
		for(int j = 0 ; j < 4 ; j++)
		{
			digitalWrite(gpioins[j], number[i][j]?HIGH:LOW);
		}
		if(i != 0)
		{
			delay(1000);
		}
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		softToneWrite(BUZZER, 330);
		delay(200);
		softToneWrite(BUZZER, 0);
		delay(200);
	}
	
}
