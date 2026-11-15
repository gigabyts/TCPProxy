#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#define BUFF_SIZE 4096
#define MAX_LISTEN_BACKLOG 1



void clientConnect(int clientSocketFd ,char* backendHostAddr, char* backendPort ) {
struct addrinfo hints;
struct addrinfo* addrs;
struct addrinfo* addrsIterable;
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

int main(int argc, char * argv[]) {
char* serverPortStr;
char* backendPortStr;
char* backendPortAddr;


struct addrinfo hints;
struct addrinfo* addrsOne;
struct addrinfo* adderLoop;
int getAddrInfoError;

int serverSocketFd;
int clientSocketFd;

int soReuseAdder;

if(argc!=4) {
    fprintf(stderr, "Usage %s <server port> <backend address> <backend port>\n",argv[0]);
    exit(1);
}
serverPortStr=argv[1];
backendPortAddr=argv[2];
backendPortStr=argv[3];
memset(&hints,0,sizeof(struct addrinfo));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
hints.ai_flags=AI_PASSIVE;
getAddrInfoError= getaddrinfo(NULL,serverPortStr,&hints,&addrsOne);

for(adderLoop=addrsOne;adderLoop!=NULL;adderLoop=adderLoop->ai_next) {

    serverSocketFd =socket(adderLoop->ai_family,adderLoop->ai_socktype,adderLoop->ai_protocol);
    if(serverSocketFd==-1) {
        continue;
    }
    soReuseAdder=1;
    setsockopt(serverSocketFd,SOL_SOCKET,SO_REUSEADDR,&soReuseAdder,sizeof(soReuseAdder));

    if(bind(serverSocketFd,adderLoop->ai_addr,adderLoop->ai_addrlen)==0){
        break;
    }
    close(serverSocketFd);
}
if(adderLoop==NULL) {
    fprintf(stderr, "Could not bind to address");
    exit(1);
}
freeaddrinfo(adderLoop);
listen(serverSocketFd,MAX_LISTEN_BACKLOG);

while(1) {
    clientSocketFd=accept(serverSocketFd,NULL,NULL);
    if(clientSocketFd==-1) {
        perror("Couldn't accept connection");
        exit(1);
    }
}
clientConnect(serverSocketFd,backendPortAddr,backendPortStr);

}