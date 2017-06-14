// basic simple socket server

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "lib.h"
#include <sys/stat.h>
#include <fcntl.h>

char * HTTP_firstline = "HTTP/1.1 200 OK\r\n";
char * HTTP_header = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
char HTTP_body[1000];

void send_file(char * url, int newsockfd){
	// get body
	FILE *fs;
	fs = fopen(url, "r");
	if (fs == NULL){
		perror("Error to open file");
		exit(1);
	}
	bzero(HTTP_body, 1000);
	fread((void *) HTTP_body, 256, 1, fs);

	int n;
	// write to the socket
	n = write(newsockfd, HTTP_firstline, strlen(HTTP_firstline));
	if (n < 0)
		error("ERROR writing to the socket");
	n = write(newsockfd, HTTP_header, strlen(HTTP_header));
	if (n < 0)
		error("ERROR writing to the socket");
	n = write(newsockfd, HTTP_body, strlen(HTTP_body));
	if (n < 0)
		error("ERROR writing to the socket");
	fclose(fs);
}

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
	printf("Server is listening.. port: %d\n\n", portno);
	int id = 0;

	while (1){
		// client is accepted
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		printf("Client %d is connecting..\n", id);

		// if(!fork())
		// {
			// close(sockfd);
			// reading from the socket
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			printf("%s\n", buffer);
			if (n < 0) 
				error("ERROR reading from socket");
			
			// if (!strncmp(buffer, "GET /style.css", 14)){
			// 	send_file("style.css", newsockfd);
			// 	printf("SENDING style.css..\n\n");
			// }
			// else if (!strncmp(buffer, "GET /", 5)){
				send_file("index.html", newsockfd);
				printf("SENDING index.html..\n\n");
			// }
			// printf("fd: %d\n", newsockfd);
			//close(newsockfd);
		// }
		//close(newsockfd);
		id++;
	}
	return 0;

}