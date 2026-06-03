#include <wiringPi.h>
#include <softTone.h>

#define BUZZER 28
#define TOTAL 32

// melody
#define NOTE_C3 131
#define NOTE_Cs3 139
#define NOTE_D3 147
#define NOTE_Ds3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_Fs3 185
#define NOTE_G3 196
#define NOTE_Gs3 208
#define NOTE_A3 220
#define NOTE_As3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_Cs4 277
#define NOTE_D4 294
#define NOTE_Ds4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_Fs4 370
#define NOTE_G4 391
#define NOTE_Gs4 415
#define NOTE_A4 440
#define NOTE_As4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_Cs5 554
#define NOTE_D5 587
#define NOTE_Ds5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_Fs5 740
#define NOTE_G5 784
#define NOTE_Gs5 831
#define NOTE_A5 880
#define NOTE_As5 923
#define NOTE_B5 988

int musicPlay()
{
	int melody[] = {
		NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, \
		NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_D4, 0, \
		NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, \
		NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_C4, NOTE_C4, 0
	};

	for(int i = 0 ; i < TOTAL ; i++)
	{
		softToneWrite(BUZZER, melody[i]);
		delay(280);
	}

	return 0;
}

void buzzer_control()
{
	musicPlay();
}
