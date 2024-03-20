#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

void usage(char *argv)
{
    printf("Usage : %s [port]\n", argv);
    printf("Example : %s 5100\n", argv);
}

int main(int argc, char** argv)
{
   int   ssock, csock;
   int   readlen;
   socklen_t clen;
   
   struct sockaddr_in servaddr, cliaddr;
   char   mesg[BUFF_SIZE];

    if(argc != 2)
	{
        usage(argv[0]);
		return -1;
    }
	
	if((ssock  = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror( "socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(ssock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind()");
		return -1;
	}

	if(listen(ssock, 5) < 0)
	{
		perror("listen()");
		return -1;
	}

	int count = 1;
	
	do
	{
		clen = sizeof(clen);
		if((csock = accept(ssock, (struct sockaddr *)&cliaddr, &clen)) < 0)
		{
			perror("accept()");
			return -1;
		}
		
		inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFF_SIZE);
		printf("%d : Client is connected : %s\n", count, mesg);
		count++;
		
		if((readlen = read(csock, mesg, BUFF_SIZE)) <= 0)
			perror("read()");
		printf("Received data : %s\n", mesg);
		
		if(write(csock, mesg, readlen) <= 0)
			perror("write()");
		
		close(csock);
	} while(strncmp(mesg, "q", 1));
	
	close(ssock);
	
	return 0;
}
