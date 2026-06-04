#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define CDS 27

#define BRIGHT 1
#define DARK 2

int* cds_control(char* mode)
{
	int* ret = (int*)malloc(sizeof(int));
	printf("%s\n", mode);
	if(strcmp(mode, "31") == 0)
	{
		printf("2. test\n");
		if(digitalRead(CDS) == HIGH)	
		{
			*ret = BRIGHT;
		}
		else
		{
			*ret = DARK;
		}
		printf("3. test\n");
		return ret;
	}
	else if(strcmp(mode, "32") == 0)
	{
		return 0;
	}
}
