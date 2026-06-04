// CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define MAXDATASIZE 10

char* toString(int a)
{
	char* ret = malloc(sizeof(char) * 3);

	ret[2] = '\0';
	ret[1] = ((a%10) + '0');
	a /= 10;
	ret[0] = (a + '0');
	
	return ret;
}

int selectLED()
{
	int mode_val;
	printf("\n---------- LED Control Mode ----------\n");
	printf("Select your mode: \n");
	printf("1. LED 100%% ON\n");
	printf("2. LED 70%% ON\n");
	printf("3. LED 50%% ON\n");
	printf("4. LED 25%% ON\n");
	printf("5. LED OFF\n");
	
	printf("What do you want to see: ");
	scanf("%d", &mode_val);

	return mode_val;
}

int selectBUZZER()
{
	int mode_val;
	printf("\n---------- BUZZER Control Mode ----------\n");
	printf("Select your mode: \n");
	printf("1. BUZZER ON\n");
	printf("2. BUZZER OFF\n");
	
	printf("What do you want to see: ");
	scanf("%d", &mode_val);

	return mode_val;
}

int selectCDS()
{
	int mode_val;
	printf("\n---------- CDS Control Mode ----------\n");
	printf("Select your mode: \n");
	printf("1. Check CDS value\n");
	printf("2. Control LED with CDS\n");

	printf("What do you want to see: ");
	scanf("%d", &mode_val);

	return mode_val;
}

int selectFND()
{
	int mode_val;
	printf("\n---------- FND Control Mode ----------\n");
	printf("Select Integer Number(0~9): ");
	scanf("%d", &mode_val);

	return mode_val;
}

int selectMode()
{
	int mode_val;
	printf("\n========== Device Management System ==========\n");
	printf("Select your mode: \n");
	printf("1. LED ON_OFF\n"); // 1X
	printf("2. Buzzer\n"); // 20
	printf("3. CDS\n"); // 3X
	printf("4. 7-Segment\n"); // 4X
	printf("5. Temperature\n"); // 50
	printf("0. EXIT\n"); // 00

	printf("What do you want to see: ");
	scanf("%d", &mode_val);

	mode_val *= 10;

	switch(mode_val)
	{
		case 10:
			mode_val += selectLED();
			break;
		case 20:
			mode_val += selectBUZZER();
			break;
		case 30:
			mode_val += selectCDS();
			break;
		case 40:
			mode_val += selectFND();
			break;
		case 50:
			break;
		default:
			mode_val = 0;
			break;
	}

	return mode_val;
}

int main(int argc, char* argv[])
{
	int sockfd, numbytes;
	socklen_t addr_len;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in server_addr;

	int mode_value;

	// signal control
	sigset_t set;

	sigfillset(&set);
	sigdelset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);

	if(argc != 2)
	{
		fprintf(stderr, "usage: client hostname\n");
		exit(1);
	}
	if((he = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(60000);
	server_addr.sin_addr = *((struct in_addr*)he->h_addr);
	printf("[%s]\n", (char*)inet_ntoa(server_addr.sin_addr));
	memset(&(server_addr.sin_zero), '\0', 8);

	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}

	while(1)
	{
		mode_value = selectMode();
		if(mode_value == 0)
		{
			printf("\n========== BYE!! ==========\n");
			return 0;
		}
		strncpy(buf, toString(mode_value), 3);
		printf("buf: %s\n", buf);


		if(send(sockfd, buf, 3, 0) == -1)
		{
			perror("send");
			close(sockfd);
			exit(1);
		}
		
		if(buf[0] == '3')
		{
			if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
			{
				perror("recv");
				close(sockfd);
				exit(1);
			}
			buf[numbytes] = '\0';
			printf("CURRENT LIGHT STATE: %s\n", buf);
		}
		if(buf[0] == '5')
		{
			if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
			{
				perror("recv");
				close(sockfd);
				exit(1);
			}
			buf[numbytes] = '\0';
			printf("CURRENT TEMPERATURE: %s\n", buf); 
		}
	}
	close(sockfd);

	return 0;
}
