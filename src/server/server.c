// SERVER 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <dlfcn.h>
#include <limits.h>
#include <libgen.h>
#include <syslog.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <wiringPiI2C.h>

#define BACKLOG 10
#define MAXDATASIZE 100

#define LED 29
#define BUZZER 28
#define CDS 27
#define I2CADDR 0x48

char lib_path[PATH_MAX];

int getLibDir()
{
	char exe_path[PATH_MAX];

	ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);

	if(len != -1)
	{
		exe_path[len] = '\0';

		char* dir = dirname(exe_path);

		sprintf(lib_path, "%s/../lib/", dir);

		return 0;
	}
	else
	{
		return -1;
	}
}

void* temp_thread(void* arg)
{
	char temp_lib[PATH_MAX+20];
	int ret;
	void* handle;
	int* (*fptr)(int);

	sprintf(temp_lib, "%slibtemp.so", lib_path);
	handle = dlopen(temp_lib, RTLD_LAZY);
	if(!handle)
	{
		syslog(LOG_ERR, "%s\n", dlerror());
		exit(1);
	}

	fptr = dlsym(handle, "temp_control");
	ret = *(fptr(*((int*)arg)));

	dlclose(handle);

	pthread_exit(&ret);
}

void* fnd_thread(void* arg)
{
	char fnd_lib[PATH_MAX+20];
	void* handle;
	void (*fptr)(char*);

	sprintf(fnd_lib, "%slibfnd.so", lib_path);
	handle = dlopen(fnd_lib, RTLD_LAZY);
	if(!handle)
	{
		syslog(LOG_ERR, "%s\n", dlerror());
		exit(1);
	}

	fptr = dlsym(handle, "fnd_control");
	fptr((char*)arg);

	dlclose(handle);
}

void* cds_thread(void* arg)
{
	char cds_lib[PATH_MAX+20];
	int* ret;
	void *handle;
	int* (*fptr)(char*);

	sprintf(cds_lib, "%slibcds.so", lib_path);
	handle = dlopen(cds_lib, RTLD_LAZY);
	if(!handle)
	{
		syslog(LOG_ERR, "%s\n", dlerror());
		exit(1);
	}

	fptr = dlsym(handle, "cds_control");
	ret = fptr((char*)arg);

	dlclose(handle);

	pthread_exit(ret);
}

void* buzzer_thread(void* arg)
{
	char buzzer_lib[PATH_MAX+20];
	void *handle;
	void (*fptr)(char*);

	sprintf(buzzer_lib, "%slibbuzzer.so", lib_path);
	handle = dlopen(buzzer_lib, RTLD_LAZY);
	if(!handle)
	{
		syslog(LOG_ERR, "%s\n", dlerror());
		exit(1);
	}
	fptr = dlsym(handle, "buzzer_control");
	fptr(arg);

	dlclose(handle);
}

void* led_thread(void* arg)
{
	char led_lib[PATH_MAX+20];
	void *handle;
	void (*fptr)(char*);

	sprintf(led_lib, "%slibled.so", lib_path);
	handle = dlopen(led_lib, RTLD_LAZY);
	if(!handle)
	{
		syslog(LOG_ERR, "%s\n", dlerror());
		exit(1);
	}
	fptr = dlsym(handle, "led_control");
	fptr((char*)arg);

	dlclose(handle);
}

void makedaemon()
{
	struct sigaction sa;
	struct rlimit rl;
	int fd0, fd1, fd2;
	pid_t pid;

	umask(0);

	if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
	{
		perror("getlimit");
		exit(1);
	}

	if((pid = fork()) < 0)
	{
		perror("fork");
		exit(1);
	}
	// close parent process
	else if(pid != 0)
	{
		_exit(EXIT_SUCCESS);
	}

	setsid();

	// signal handler
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(sigaction(SIGHUP, &sa, NULL) < 0)
	{
		perror("sigaction(): Can't ignore SIGHUP");
		exit(1);
	}

	if(chdir("/"))
	{
		perror("cd");
		exit(1);
	}

	// close all file descriptor
	if(rl.rlim_max == RLIM_INFINITY)
	{
		rl.rlim_max = 1024;
	}

	for(int i = 0 ; i < rl.rlim_max ; i++)
	{
		close(i);
	}

	// make fd 0, 1, 2 to /dev/null
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	// log open
	openlog("device_control", LOG_CONS, LOG_DAEMON);
	if(fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
    }

    syslog(LOG_INFO, "Daemon Process");
}

int wpiSetup(int* i2c_fd)
{
	if(wiringPiSetup() == -1)	
	{
		syslog(LOG_ERR, "wiringPiSetup");
		return -1;
	}

	if(softPwmCreate(LED, 0, 255) == -1)
	{
		syslog(LOG_ERR, "softPwmCreate");
		return -1;
	}

	if(softToneCreate(BUZZER) == -1)
	{
		syslog(LOG_ERR, "softToneCreate");
		return -1;
	}

	if((*i2c_fd = wiringPiI2CSetup(I2CADDR)) == -1)
	{
		syslog(LOG_ERR, "I2Csetup");
		return -1;
	}

	pinMode(CDS, INPUT);

	return 0;
}

int main()
{
	int sockfd, new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size;
	char buf[MAXDATASIZE];
	int numbytes;
	void* cds_val;
	void* temp_val;


	int i2c_fd;

	pthread_t led_tid, buzzer_tid, cds_tid, fnd_tid, temp_tid;

	if(getLibDir() == -1)
	{
		syslog(LOG_ERR, "library bind error");
		exit(1);
	}

	// make process daemon
	makedaemon();

	// setup wiringPi
	if(wpiSetup(&i2c_fd) == -1)
	{
		syslog(LOG_ERR, "wpiSetup");
		exit(1);
	}

	// socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		syslog(LOG_ERR, "socket");
		exit(1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(60000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), '\0', 8);

	// bind
	int optvalue = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));
	if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		syslog(LOG_ERR, "bind");
		exit(1);
	}

	// listen
	if(listen(sockfd, BACKLOG) == -1)
	{
		syslog(LOG_ERR, "listen");
		exit(1);
	}

	// accept
	sin_size = sizeof(struct sockaddr_in);
	if((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) == -1)
	{
		syslog(LOG_ERR, "accept");
		exit(1);
	}
	syslog(LOG_INFO, "server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));

	while(1)
	{
		if((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1)
		{
			syslog(LOG_ERR, "recv");
			exit(1);
		}
		buf[numbytes] = '\0';
		syslog(LOG_INFO, "input: %s", buf);

		// led
		if(buf[0] == '1')
		{
			syslog(LOG_INFO, "led thread start");
			pthread_create(&led_tid, NULL, led_thread, buf);
			pthread_detach(led_tid);
			syslog(LOG_INFO, "led thread detach");
		}
		// buzzer
		if(buf[0] == '2')
		{
			if(buf[1] == '1')
			{
				syslog(LOG_INFO, "buzzer thread start");
				pthread_create(&buzzer_tid, NULL, buzzer_thread, NULL);
				pthread_detach(buzzer_tid);
				syslog(LOG_INFO, "buzzer thread detach");
			}
			else if(buf[1] == '2' && (pthread_kill(buzzer_tid, 0)) == 0)
			{
				syslog(LOG_INFO, "buzzer stop\n");
				pthread_cancel(buzzer_tid);
				softToneWrite(BUZZER, 0);
			}
		}
		// cds
		if(buf[0] == '3')
		{
			syslog(LOG_INFO, "cds thread start");
			pthread_create(&cds_tid, NULL, cds_thread, buf);
			pthread_join(cds_tid, &cds_val);
			if(*((int*)cds_val) == 1)
			{
				strcpy(buf, "BRIGHT");
				syslog(LOG_INFO, "cds: bright");
			}
			else if(*((int*)cds_val) == 2)
			{
				strcpy(buf, "DARK");
				syslog(LOG_INFO, "cds: dark");
			}
			else
			{
				strcpy(buf, "WRONG");
				syslog(LOG_WARNING, "wrong cds input");
			}
			
			send(new_fd, buf, strlen(buf), 0);
			syslog(LOG_INFO, "send cds data");
		}
		// fnd
		if(buf[0] == '4')
		{
			syslog(LOG_INFO, "fnd trhead start: inputNUM=%c", buf[1]);
			pthread_create(&fnd_tid, NULL, fnd_thread, buf);
			pthread_detach(fnd_tid);
		}
		// temperature
		if(buf[0] == '5')
		{
			syslog(LOG_INFO, "temperature thread start");
			pthread_create(&temp_tid, NULL, temp_thread, &i2c_fd);
			pthread_join(temp_tid, &temp_val);
			syslog(LOG_INFO, "temperature thread finish");
			sprintf(buf, "%d", *((int*)temp_val));

			send(new_fd, buf, strlen(buf), 0);
			syslog(LOG_INFO, "send temperature data");
		}
		if(buf[0] == '0')
		{
			syslog(LOG_INFO, "client LOGOUT");
			close(new_fd);

			if((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) == -1)
			{
				syslog(LOG_ERR, "accept");
				exit(1);
			}
			syslog(LOG_INFO, "server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));
		}
	}

	closelog();
	return 0;
}
