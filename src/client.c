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

#define MAXDATASIZE 100

int main(int argc, char* argv[])
{
	int sockfd, numbytes;
	socklen_t addr_len;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in server_addr;

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
		if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		buf[numbytes] = '\0';
		printf("Received : %s\n", buf);
	}
	close(sockfd);

	return 0;
}
