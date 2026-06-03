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

#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#define BACKLOG 10
#define MAXDATASIZE 100

#define LED 29
#define BUZZER 28

void* led_thread(void* arg)
{

	void *handle;
	void (*fptr)(char*);

	handle = dlopen("../lib/libled.so", RTLD_LAZY);
	if(!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
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
}

int wpiSetup()
{
	if(wiringPiSetup() == -1)	
	{
		perror("wiringPiSetup");
		return -1;
	}

	if(softPwmCreate(LED, 0, 255) == -1)
	{
		perror("softPwmCreate");
		return -1;
	}

	if(softToneCreate(BUZZER) == -1)
	{
		perror("softToneCreate");
		return -1;
	}

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

	pthread_t led_tid, buzzer_tid, cds_tid, seg_tid;

	// wiringPi setup
	if(wpiSetup() == -1)
	{
		perror("wpiSetup");
		exit(1);
	}

	// make process daemon
	//makedaemon();

	// socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
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
		perror("bind");
		exit(1);
	}

	// listen
	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	// accept
	sin_size = sizeof(struct sockaddr_in);
	if((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) == -1)
	{
		perror("accept");
		exit(1);
	}
	printf("server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));

	while(1)
	{
		if((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		buf[numbytes] = '\0';

		if(buf[0] == '1')
		{
			pthread_create(&led_tid, NULL, led_thread, buf);
		}
		pthread_join(led_tid, (void**)NULL);
	}
	return 0;
}
