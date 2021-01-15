#include "../headers/RequestManager.hpp"

RequestManager::RequestManager(/* args */)
{
}

RequestManager::~RequestManager()
{
}

RequestManager::RequestManager(ClientManager* clientManager, std::mutex* clientManagerLock,std::deque <Request*>* requestQue ,pthread_mutex_t* requestQueLock,pthread_cond_t *gotRequest ){
    this->clientManager=clientManager;
    this->clientManagerLock=clientManagerLock;
    this->requestQue=requestQue;
    this->requestQueLock=requestQueLock;
    this->gotRequest=gotRequest;

}
int RequestManager::checkIfIsConected(int sd){
    char *x =new char();
    if(recv(sd,x,1,MSG_PEEK)<=0){
        return 0;
    }
    printf("[%d]dupa rcv\n",sd);
    return 1;
}
Request * RequestManager::decodeRequest(int sd){
    Request* newRequest = new Request();
    newRequest->setClient(sd);
    int rqType;
    if(read(sd,&rqType,sizeof(int))==-1)return NULL;
    newRequest->setRequestType(rqType);
    if(rqType==LOGIN||rqType==UPDATE){
        int len;
        if(read(sd,&len,sizeof(int))==-1)return NULL;
        char* parameter = (char*)malloc(len);
        if(read(sd,parameter,len)==-1)return NULL;
        newRequest->setRequestParameter(parameter);
    }
    return newRequest;
}
int RequestManager::reciveMesage(int sd){
    Request* newRequest = decodeRequest(sd);
        pthread_mutex_lock(this->requestQueLock);
        this->requestQue->push_back(newRequest);
        pthread_cond_signal(this->gotRequest);
        pthread_mutex_unlock(this->requestQueLock);
    if(newRequest!=NULL)return 1;
    else return 0;

}


void RequestManager::start(){
    printf("[Thread Request Manager]start\n");
    fd_set readfds;
    struct timeval tv;
    int retval;
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    
    int maxfd=0;
    

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
            perror("[Thread Request Manager]Eroare la select()\n");
        }
        else{
            if(retval){
                for(int clientSD : clientSDList){
                    if(FD_ISSET(clientSD,&readfds)){
                        printf("[SD set]%d\n",clientSD);
                        //if(read(clientSD,&fromClient,sizeof(int))==0){
                        if(reciveMesage(clientSD)==0){
                            this->clientManagerLock->lock();
                            this->clientManager->removeClient(clientSD);
                            close(clientSD);
                            this->clientManagerLock->unlock();
                        }
                        else{
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