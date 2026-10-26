#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char ** argv)
{
    unsigned short port;
    char bufStore[25];
    int hostResol;
    struct addrinfo *hostName;
    struct sockaddr_in server;
    int cliSocket;

    if(argc!=3)
    {
        fprintf(stderr,"Usage: %s hostname port\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    hostResol=getaddrinfo(NULL,argv[1],NULL,&hostName);
    if(hostResol!=0)
    {
        fprintf(stderr,"getaddrinfo: %s \n",gai_strerror(hostResol));
        exit(EXIT_FAILURE);
    }

    port = (unsigned short)atoi(argv[2]);
    strcpy(bufStore,"Socket Messages are cool");

    server.sin_family = AF_INET;
    server.sin_port=htons(port);

}