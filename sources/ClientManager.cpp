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
std::list<int> ClientManager::getClientSDList(){
    std::list<int> l;
    for(ClientData cl :this->clientList){
        l.push_front(cl.getSD());
    }
    return l;
}

void ClientManager::removeClient(int clientSD){
    this->clientList.remove_if(
        [clientSD](ClientData cl){
            return cl.getSD()==clientSD;
        }
    );
}