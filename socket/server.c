// basic simple socket server

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen, n;
	char buffer[256];
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

	while (1){
		// client is accepted
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		printf("Client %d is connecting..\n", id);

		// reading from the socket
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) 
			error("ERROR reading from socket");
		printf("Client %d: %s\n", id, buffer);

		// write to the socket
		n = write(newsockfd, "I got your message", 18);
		if (n < 0)
			error("ERROR writing to the socket");
		id++;
	}
	return 0;

}