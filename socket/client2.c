// basic simple socket client -- ver2

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "lib.h"
#include <netdb.h>
#include <pthread.h>

pthread_t th_read, th_write;
char buffer[256];
void * func_read(void * sockfd){
	// read from the socket
	while(1){
		bzero(buffer, 256);
		int n = read((int) sockfd, buffer, 255);
		if (n < 0)
			error("ERROR reading from socket");
		printf("SERVER: %s", buffer);
	}
}

void * func_write(void * sockfd){
	// write to the socket
	while(1){
		printf("CLIENT: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		int n = write((int) sockfd, buffer, strlen(buffer));
		if(n < 0)
			error("ERROR writing to socket");
	}
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port", argv[0]);
		exit(0);
	}

	// get port
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	// get server
	server = gethostbyname(argv[1]);
	if(server == NULL)\
	{
		fprintf(stderr, "ERROR, no such host");
		exit(0);
	}

	// configure address
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	// connect to server
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");
		
	int err1, err2;
	// Create thread 1, 2
	err1 = pthread_create(&(th_read), NULL, &func_read, (void *)sockfd);
	if (err1 != 0){
		printf("Fail to create thread Read\n");
	}
	err2 = pthread_create(&(th_write), NULL, &func_write, (void *)sockfd);
	if (err2 != 0){
		printf("Fail to create thread Write\n");
	}
	pthread_join(th_read, NULL);
	pthread_join(th_write, NULL);
	return 0;


}
