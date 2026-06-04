#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define I2CADDR 0x48

int* temp_control(int* fd)
{
	int temp_fd = *fd;
	int* ret = (int*)malloc(sizeof(int));
	int raw_adc;

	float r1, rt, t0, b, r0;
	float kelvin;
	int celcious;


	wiringPiI2CWrite(temp_fd, 1);
	wiringPiI2CRead(temp_fd); // throw 1st data
	raw_adc = 255 - wiringPiI2CRead(temp_fd);

	r1 = 10000.0;
	rt = r1 * ((255.0 / (float)raw_adc) - 1.0);

	t0 = 298.15;
	b = 3950.0;
	r0 = 100000.0;

	kelvin = 1.0 / (1.0 / t0 + (1.0 / b) * log(rt / r0));
	celcious = kelvin - 273.15;
	
	*ret = celcious;
	return ret;
}

