# Device Management System test

<div align=center>

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Raspi](https://img.shields.io/badge/raspberrypi-A22846.svg?style=for-the-badge&logo=raspberrypi&logoColor=black)
![Makefile](https://img.shields.io/badge/Makefile-427819?style=for-the-badge&logo=gnu&logoColor=white)
![Linux](https://img.shields.io/badge/ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=black)

![POSIX Threads](https://img.shields.io/badge/POSIX-Pthreads-blue?style=for-the-badge)
![Shared Library](https://img.shields.io/badge/dlopen-dlsym-green?style=for-the-badge)

</div>

## 개요
본 프로젝트는 리눅스 클라이언트(이하 클라이언트) 에서 라즈베리파이 서버(이하 서버) 에 각종 장치를 제어하는 프로젝트 입니다. \
빌드를 위해서는 Make를 이용합니다. \

본 프로젝트에서 제어하는 장치는 다음 5가지 입니다.
- LED
- 부저
- 조도 센서
- 7 segment(FND)
- AD/DA 컨버터

## 빌드 방법
Makefile을 통해 빌드를 진행합니다. \
빌드 순서는 다음과 같습니다.
``` bash
$ make

```


## 사용 방법


## 기능
### LED
PWM 제어를 통해 LED 밝기 제어(0%, 25%, 50%, 75%, 100%)
### 부저
부저를 통해 소리 ON/OFF
### 조도 센서
현재 밝기를 측정하여 클라이언트에 반환(밝은지 어두운지 파악) \
현재 밝기에 따라 LED 제어
### 7 segment(FND)
사용자가 원하는 1자리 숫자 입력시 입력한 시간동안 매 초 1씩 감소 \
이후, 0이 되면서 부저에서 3번 소리 재생
### AD/DA 컨버터
현 온도를 확인하여 클라이언트에 반환