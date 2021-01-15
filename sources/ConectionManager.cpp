#include "../headers/ConectionManager.hpp"

ConectionManager::ConectionManager(/* args */)
{
}

ConectionManager::~ConectionManager()
{
}
ConectionManager::ConectionManager(ClientManager * clientManager){
    this->clientManager=clientManager;
}
int ConectionManager::start(int sd){
    printf("[Thread Conection Manager]start\n");
    printf("[Thread Conection Manager]Se face listen la : %d\n",sd);
    int client;
    struct sockaddr_in from;
    bzero(&from,sizeof(from));
    int length = sizeof(from);
    while(true){
        
        if((client=accept(sd,(struct sockaddr*)&from,(socklen_t*)&length))<0){
            printf("[Server]Eroare la accept()\n");
        }
        else{
            ClientData newClient(client);
            clientManager->addClient(newClient);
        }
        
    }
    
}
