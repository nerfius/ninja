/**
 * based on source code at
 * http://www.cs.dartmouth.edu/~campbell/cs60/socketprogramming.html
 * */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096
#define SERV_PORT 3000

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in serv_adr;
	char sendline[MAXLINE], recvline[MAXLINE];

	printf("client started...\n");

	if (argc != 2) {
		perror("Usage: TCP Client <SERVER IP>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error\n");
		exit(2);
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(SERV_PORT);

	if (connect(sockfd, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0) {
		perror("could not connect to server...\n");
		exit(3);
	}

	printf("me: ");
	while (fgets(sendline, MAXLINE, stdin) != NULL) {
		send(sockfd, sendline, strlen(sendline), 0);
		if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
			perror("unexpected server shutdown\n");
			exit(4);
		}
		printf("server: ");
		fputs(recvline, stdout);
		printf("me: ");
		memset(recvline, 0, sizeof(recvline));
		memset(sendline, 0, sizeof(sendline));
	}

	printf("client shutting down...\n");
	return 0;
}
