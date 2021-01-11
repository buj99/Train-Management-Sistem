#include "../headers/ClientManager.hpp"

ClientManager::ClientManager(/* args */)
{
}

ClientManager::~ClientManager()
{
}

std::list<ClientData>* ClientManager::getClientList(){
    return NULL;
}
bool ClientManager::addClient(ClientData client){
    printf("[Client Manager]client nou: %d\n",client.getSD());
    clientList.push_front(client);

    return true;
}
