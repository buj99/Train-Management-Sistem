#include "../headers/ClientManager.hpp"

ClientManager::ClientManager(pthread_mutex_t * clientListLock)
{
    this->clientListLock=clientListLock;
}

ClientManager::~ClientManager()
{
}

std::list<ClientData*>* ClientManager::getClientList(){
    return &(this->clientList);
}
bool ClientManager::addClient(ClientData client){
    //mutex
    printf("[Client Manager]client nou: %d\n",client.getSD());
    pthread_mutex_lock(this->clientListLock);
    clientList.push_front(&client);
    pthread_mutex_unlock(this->clientListLock);
    return true;
}
std::list<int> ClientManager::getClientSDList(){
    std::list<int> l;
    for(ClientData* cl :this->clientList){
        l.push_front(cl->getSD());
    }
    return l;
}

void ClientManager::removeClient(int clientSD){
    //mutex
    pthread_mutex_lock(this->clientListLock);
    this->clientList.remove_if(
        [clientSD](ClientData* cl){
            return cl->getSD()==clientSD;
        }
    );
    pthread_mutex_unlock(this->clientListLock);
}
int ClientManager::getTrainId(int clientSD){
    for(ClientData* cl : clientList){
        if(cl->getSD()==clientSD){
            return cl->getTrainId();
        }
    }
}
bool ClientManager::clientIsLogedIn(int sd){
    for(ClientData* cl: clientList){
        if(cl->getSD()==sd){
            if(cl->getTrainId()==-1)return false;//client nu este logat
            else return true;  //clientul este logat
        }
    }
    return true;
}
void ClientManager::asignTrainId(int sd,int trainId){
    for(ClientData* cl: clientList){
        if(cl->getSD()==sd)
        {
            cl->setTrainId(trainId);
        }
    }
}