#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdbool.h>
#include <list>
#include <deque>
#include <mutex>



#include "headers/ConectionManager.hpp"
#include "headers/ClientManager.hpp"
#include "headers/RequestManager.hpp"
#include "./headers/Request.hpp"


#define PORT 2020
#define NOTHREADS 100

extern int errno;

std::mutex clientManagerLock;
std::list<ClientData> clientList;

std::deque <Request *> requestQue;
std::mutex requestQueLock;
int sd; //soketul pe care se asculta pt accept

typedef struct {
    pthread_t idThread;
    int thCount;
}Thread; 
Thread* threadsPool;
void  threadCreate(int i);
static void * treat(void*);

ClientManager clientManager;
pthread_t conectionManagerThread;
static void * treatConectionManagerThread(void* arg);

pthread_t requestManagerThread;
static void * treatRequestManagerThread(void*arg);

int main (){

    struct sockaddr_in server ;
    threadsPool=(Thread*)calloc(sizeof(Thread),NOTHREADS);
    if((sd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    int on=1;

    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(PORT);


    if(bind(sd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){
        perror("[Server]Eroare la bind()\n");
        return errno;
    }
    if(listen(sd,10)==-1){
        perror("[Server]Eroare la listen()\n");
        return errno;
    }

    for(int i=0;i<NOTHREADS;i++) threadCreate(i);
    printf("[Server]Astept clienti la portul %d \n",PORT);
    pthread_create(&conectionManagerThread,NULL,&treatConectionManagerThread,(void*)&sd);
    pthread_create(&requestManagerThread,NULL,&treatRequestManagerThread,NULL);
    while(true);
    return 0;
}

void  threadCreate(int i){
    pthread_create(&threadsPool[i].idThread,NULL,&treat,(void*)&i);
}
static void * treat(void* arg){
    int i =*((int*)arg);
    while(true){
        if(requestQue.front()!=NULL){
            requestQueLock.lock();
            Request* rqToTreat = requestQue.front();
            requestQue.pop_front();
            requestQueLock.unlock();
            printf("[Thread %d] Am prmit requestul : \n",i);
            rqToTreat->printInfo();
        }
        sleep(1);
    }
}

static void * treatConectionManagerThread(void* arg){
    ConectionManager conectionManager(&clientManager,&clientManagerLock);
    conectionManager.start(sd);
} 
static void* treatRequestManagerThread(void* arg){

    RequestManager requestManager(&clientManager,&clientManagerLock,&requestQue,&requestQueLock);
    requestManager.start();
    return NULL;
}