#include "../headers/RequestManager.hpp"

RequestManager::RequestManager(/* args */)
{
}

RequestManager::~RequestManager()
{
}

RequestManager::RequestManager(ClientManager* clientManager, std::mutex* clientManagerLock){
    this->clientManager=clientManager;
    this->clientManagerLock=clientManagerLock;
}
int RequestManager::reciveMessage(char** msg,int sd){
    char* msgFromClient;
    int len ;
    int readRet;
    if((readRet=read(sd, &len,sizeof(int)))==-1){
        perror("[Thread Request Manager]Eroare la read()\n");
        return -1;
    }
    if(readRet==0)return 0;
    msgFromClient=(char*)malloc(sizeof(char*)*len);
    bzero(msgFromClient,sizeof(char));
    if((readRet=read(sd,msgFromClient,len))==-1){
        perror("[Thread Request Manager]Eroare la read()\n");
        return -1;
    }
    if(readRet==len){ 
        *msg=msgFromClient;
        return len;
    }
}
void RequestManager::start(){
    printf("[Thread Request Manager]start\n");
    fd_set readfds;
    struct timeval tv;
    int retval;
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    
    int maxfd=0;
    FD_ZERO(&readfds);
    while (true){ 
        char * msgFromClient = (char*)malloc(sizeof(char)); 
        this->clientManagerLock->lock();
        clientSDList=this->clientManager->getClientSDList();
        this->clientManagerLock->unlock();
        for(int sd :clientSDList){
            FD_SET(sd,&readfds);
            if(sd>maxfd){
                maxfd=sd;
            }
        }
        retval=select(maxfd+1,&readfds,NULL,NULL,&tv);
        if(retval==-1){
            printf("[Thread Request Manager]Eroare la select()\n");
        }
        else{
            if(retval){
                for(int clientSD : clientSDList){
                    if(FD_ISSET(clientSD,&readfds)){
                        //if(read(clientSD,&fromClient,sizeof(int))==0){
                        if(reciveMessage(&msgFromClient,clientSD)==0){
                            this->clientManagerLock->lock();
                            this->clientManager->removeClient(clientSD);
                            close(clientSD);
                            this->clientManagerLock->unlock();
                        }
                        else{
                            printf("[Thread Request Manager]Am primit requestul %s\n",msgFromClient);
                        }
                        
                    }
                }
            }
        }
        FD_ZERO(&readfds);
        maxfd=0;
        free(msgFromClient);
    }
}