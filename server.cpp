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
#include <mutex>


#include "headers/ConectionManager.hpp"
#include "headers/ClientManager.hpp"
#include "headers/RequestManager.hpp"


#define PORT 2030
#define NOTHREADS 100

extern int errno;

std::mutex clientManagerLock;
std::list<ClientData> clientList;
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
int acc_con(int sd);
void checkClientRequest(std::list<ClientData> clList);
void addToRequestQue(ClientData client);
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
    //printf("[thread %d]pornit...\n",i);
}

static void * treatConectionManagerThread(void* arg){
    ConectionManager conectionManager(&clientManager,&clientManagerLock);
    conectionManager.start(sd);
} 
static void* treatRequestManagerThread(void* arg){

    RequestManager requestManager(&clientManager,&clientManagerLock);
    requestManager.start();
    return NULL;
}
// int acc_con(int sd){
//     
//     return 0;
// }

// void checkClientRequest(std::list<ClientData> clList){
//     fd_set readfds;
//     struct timeval tv;
//     int retval;
//     int lastsd;
//     tv.tv_sec=1;
//     tv.tv_usec=0;
//     FD_ZERO(&readfds);
//     for(ClientData clData:clList){
//         lastsd=clData.getSD();
//         FD_SET(lastsd,&readfds);
//     }
//     retval=select(lastsd+1,&readfds,NULL,NULL,&tv);
//     if(retval==-1){
//         perror("[server]Eroare la select()\n");
//     }
//     else{
//         if(retval){
//             for(ClientData cl : clList){
//                 if(FD_ISSET(cl.getSD(),&readfds)){
//                     addToRequestQue(cl);
//                 }
//             }
//         }
//     }
    
// }

// void addToRequestQue(ClientData client){
//     printf("[Server]request nou de la %d\n",client.getSD());
//     int primit;
//     read(client.getSD(),&primit, sizeof(primit));
//     sleep(5);
// }