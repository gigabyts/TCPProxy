#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFF_SIZE 4096
#define MAX_LISTEN_BACKLOG 1



void clientConnect(int clientSocketFd ,char* backendHostAddr, char* backendPort ) {
struct addrinfo hints;
struct addrinfo *addrs;
struct addrinfo *addrsIterable;
int locateAddrError;
int backEndSocketFd;
char buffer[BUFF_SIZE];
int bytes_read;

memset(&hints, 0,sizeof(struct addrinfo));
hints.ai_family =AF_UNSPEC;
hints.ai_socktype =SOCK_STREAM;

locateAddrError = getaddrinfo(backendHostAddr,backendPort,&hints,&addrs);
if(locateAddrError!=0) {
    fprintf(stderr,"Can't locate backend %s/n",gai_strerror(locateAddrError));
    exit(1);
}

for (addrsIterable=addrs;
    addrsIterable!=NULL;
    addrsIterable=addrs->ai_next) {
        backEndSocketFd=socket(addrsIterable->ai_family,
            addrsIterable->ai_socktype,
            addrsIterable->ai_protocol);
        if(backEndSocketFd==-1) {
            continue;;
        }

        if(connect(backEndSocketFd,addrsIterable,addrsIterable->ai_addrlen)!=-1){
            break;
        }
        close(backEndSocketFd);
    }
    if(addrsIterable==NULL) {
        fprintf(stderr, "Could not connect to backend");
        exit(1);
    }
    freeaddrinfo(addrs);
    bytes_read = read(clientSocketFd, buffer, BUFF_SIZE);
    write(backEndSocketFd, buffer, bytes_read);
    while(bytes_read = read(backEndSocketFd,buffer, BUFF_SIZE)) {
     write(clientSocketFd,buffer,bytes_read);
    
    }
    close(clientSocketFd);
}