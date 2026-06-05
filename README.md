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
빌드를 위해서는 Make를 이용합니다.

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
### 라즈베리파이에 서버 및 라이브러리 전송
라즈베리파이에 `server` 파일과 라이브러리 파일을 전송해주어야 합니다. \
전송하기 전, `Makefile`에서 `< >` 사이에 있는 부분을 본인의 서버에 맞춰 수정해 주시면 됩니다.
``` Makefile
REMOTE_USER = <SERVER_USER_NAME>
REMOTE_HOST = <SERVER_HOST_IP>
REMOTE_DIR = <SERVER_DIR>/server
```
``` bash
$ make deploy
```

## 사용 방법
### 서버
``` bash
$ cd bin
$ ./server
```

로그를 보기 위해서는 다음을 입력합니다.
``` bash
$ journalctl -f -t device_control
```
### 클라이언트
``` bash
$ cd bin
$ ./client <server IP>


========== Device Management System ==========
Select your mode:
1. LED ON_OFF
2. Buzzer
3. CDS
4. 7-Segment
5. Temperature
0. EXIT
What do you want to see:
```

## 기능
### LED
PWM 제어를 통해 LED 밝기를 조절할 수 있습니다.

``` bash
---------- LED Control Mode ----------
Select your mode:
1. LED 100% ON
2. LED 75% ON
3. LED 50% ON
4. LED 25% ON
5. LED OFF
What do you want to see:
```
### 부저
부저를 통해 소리를 켜거나 끌 수 있습니다.
``` bash
---------- BUZZER Control Mode ----------
Select your mode:
1. BUZZER ON
2. BUZZER OFF
What do you want to see:
```
### 조도 센서
현재 밝은지 어두운지를 파악할 수 있습니다. \
현재 밝기에 따라 LED를 켜거나 끌 수 있습니다.
``` bash
---------- CDS Control Mode ----------
Select your mode:
1. Check CDS value
2. Control LED with CDS
What do you want to see:
```
### 7 segment(FND)
사용자가 원하는 시간을 입력하면(0~9) 매 초 1씩 줄어듭니다. \
이후, 0이 되면서 부저에서 3번 소리를 재생합니다.
``` bash
---------- FND Control Mode ----------
Select Integer Number(0~9):
```
### AD/DA 컨버터
현 온도를 클라이언트에 반환합니다.
``` bash
What do you want to see: 5
CURRENT TEMPERATURE: 23 C
```