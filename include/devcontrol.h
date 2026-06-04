#ifndef _DEVCONTROL_H_
#define _DEVCONTROL_H_

void led_control(char* mode);
void buzzer_control();
int* cds_control(char* mode);
void fnd_control(char* mode);
int* temp_control(int fd);

#endif // _DEVCONTROL_H_
