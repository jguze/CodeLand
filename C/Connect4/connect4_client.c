#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024
#define PORT 2002
char *hostname;
struct sockaddr_in serveraddr;
int sockfd, portno, n;
char buffer[BUFSIZE];

void network_setup()
{
	/* socket: creating the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		fprintf(stderr, "ERROR opening socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( PORT );
	
	if ( connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)))
	{
		fprintf(stderr, "CLIENT: Error calling connect().\n");
		exit(EXIT_FAILURE);
	}
	
	snprintf(buffer, sizeof(buffer), "This is the client\n");
	n = write(sockfd, buffer, BUFSIZE);
	if (n < 0)
		fprintf(stderr, "CLIENT: Error write failed.\n");
	n = read(sockfd, buffer, BUFSIZE);
	if (n < 0)
		fprintf(stderr, "CLIENT: Error read failed.\n");
	printf("Server says: %s", buffer);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
       fprintf(stderr,"usage: %s <hostname> \n", argv[0]);
       exit(0);
    }	
	hostname = argv[1];
	
	network_setup();
}