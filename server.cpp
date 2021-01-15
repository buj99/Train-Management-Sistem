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
#include "headers/Request.hpp"
#include "headers/info_trenuri.hpp"
#include "headers/RequestProcesor.hpp"



#define PORT 2020
#define NOTHREADS 100


extern int errno;

std::mutex clientManagerLock;
std::list<ClientData> clientList;

std::deque <Request *> requestQue;
pthread_mutex_t requestQueLock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gotRequest=PTHREAD_COND_INITIALIZER;


int sd;

typedef struct {
    pthread_t idThread;
    int thCount;
}Thread; 

Thread* threadsPool;
void  threadCreate(int i);
static void * treat(void*);


pthread_mutex_t clientListLock;
ClientManager clientManager(&clientListLock);
pthread_t conectionManagerThread;
static void * treatConectionManagerThread(void* arg);

pthread_t requestManagerThread;
static void * treatRequestManagerThread(void*arg);


InfoTrenuri infoT;
char xml_location[]="test.xml";

RequestManager requestManager(&clientManager,&clientManagerLock,&requestQue,&requestQueLock,&gotRequest);
int main (){

    infoT.initiate(xml_location);

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
    RequestProcesor rqProcesor(&clientManager,&infoT);
    Request* rqToTreat;
    int rc;
    while(true){
        pthread_mutex_lock(&requestQueLock);
        if(requestQue.front()==NULL){
            pthread_cond_wait(&gotRequest,&requestQueLock);
        }
        rqToTreat=requestQue.front();
        requestQue.pop_front();
        pthread_mutex_unlock(&requestQueLock);
        rqProcesor.processRequest(*rqToTreat);
    }
}

static void * treatConectionManagerThread(void* arg){
    ConectionManager conectionManager(&clientManager);
    conectionManager.start(sd);
} 
static void* treatRequestManagerThread(void* arg){

    RequestManager requestManager(&clientManager,&clientManagerLock,&requestQue,&requestQueLock,&gotRequest);
    requestManager.start();
    return NULL;
}
