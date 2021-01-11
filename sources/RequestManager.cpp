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

void RequestManager::start(){
    printf("[Thread Request Manager]start\n");
    fd_set readfds;
    struct timeval tv;
    int retval;
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    int fromClient;
    int maxfd=0;
    FD_ZERO(&readfds);
    while (true){   
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
                        if(read(clientSD,&fromClient,sizeof(int))==0){
                            this->clientManagerLock->lock();
                            this->clientManager->removeClient(clientSD);
                            close(clientSD);
                            this->clientManagerLock->unlock();
                        }
                        else{
                            printf("[Thread Request Manager]De la  %d\n",clientSD);
                        }
                        
                    }
                }
            }
        }
        FD_ZERO(&readfds);
        maxfd=0;
    }
}