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
#define LISTENQ 8

int main(int argc, char *argv[])
{
	int listenfd, connfd, n;
	pid_t childpid;
	socklen_t cli_len;
	char buf[MAXLINE];
	struct sockaddr_in cli_adr, serv_adr;
	printf("server started...\n");
	/**
	 * create a socket, socket()
	 * bind socket to address, bind()
	 * listen for connections, listen()
	 * accept connections, accept() (blocks)
	 * send(), receive()
	 * close the connection
	 * */

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("socket error\n");
		//return 1;
		exit(2);
	}

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (struct sockaddr *)&serv_adr, sizeof(serv_adr)))
	{
		perror("bind error\n");
	}

	printf("server is listening...\n");
	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		cli_len = sizeof(cli_adr);
		// accept a connection
		connfd = accept(listenfd, (struct sockaddr *)&cli_adr, &cli_len);
		printf("request recieved...\n");

		if ((childpid = fork()) == 0) {
			printf("child process created for client requests\n");
			close(listenfd);
			while ((n = recv(connfd, buf, MAXLINE, 0)) > 0) {
				printf("client: ");
				printf("%s", buf);
				send(connfd, buf, n, 0);
				memset(buf, 0, sizeof(buf));
			}
			if (n < 0) {
				printf("READ ERROR\n");
				exit(0);
			}
		}
		close(connfd);
	}

	printf("server shutting down...\n");
	return 0;
}
