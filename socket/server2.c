// basic simple socket server -- ver 2

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"
#include <pthread.h>

struct arg_struct{
	int arg1;
	int arg2;
};

pthread_t th_read, th_write;
char buffer[256];
void * func_read(void *argument){
	// reading from the socket
	struct arg_struct *args = argument;
	int newsockfd = args -> arg1;
	int id = args -> arg2;
	while(1){
		bzero(buffer, 256);
		int n = read(newsockfd, buffer, 255);
		if (n < 0) 
			error("ERROR reading from socket");
		printf("Client %d: %s", id, buffer);
	}
}

void * func_write(void *argument){
	// write to the socket
	struct arg_struct *args = argument;
	int newsockfd = args -> arg1;
	int id = args -> arg2;
	while(1){
		printf("SERVER: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		int n = write(newsockfd, buffer, 255);
		if (n < 0)
			error("ERROR writing to the socket");
	}
}


int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen, n;
	struct sockaddr_in serv_addr, cli_addr;

	// get the input port
	if (argc < 2)
	{
		fprintf(stderr, "%s\n", "ERROR, no port provided");
		exit(1);
	}

	// open a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0){
		error("ERROR opening socket");
	}

	// configure address
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// binding port
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	// socket listen
	listen(sockfd, 5);
	printf("Server is listening.. port: %d\n", portno);
	int id = 0;

	//while (1){
		// client is accepted
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		printf("Client %d is connecting..\n", id);

		while (10){
			// reading from the socket
			// (void *)func_read(newsockfd, id);
			// (void *)func_write(newsockfd, id);

			struct arg_struct args;
			args.arg1 = newsockfd;
			args.arg2 = id;

			int err1, err2;
			// Create thread 1, 2
			err1 = pthread_create(&(th_read), NULL, &func_read, (void *)&args);
			if (err1 != 0){
				printf("Fail to create thread Read\n");
			}
			err2 = pthread_create(&(th_write), NULL, &func_write, (void *)&args);
			if (err2 != 0){
				printf("Fail to create thread Write\n");
			}
			pthread_join(th_read, NULL);
			pthread_join(th_write, NULL);
		}
		// id++;
	//}
	return 0;

}