
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
    while (true)
    {
        /* code */
    }
    
}