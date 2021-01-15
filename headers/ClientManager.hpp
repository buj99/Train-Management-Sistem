#pragma once
#include <stdio.h>
#include "ClientData.hpp"
#include <list>
#include <iterator>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class ClientManager
{
private:
    std::list<ClientData*> clientList;
    pthread_mutex_t * clientListLock;
public:
    ClientManager(pthread_mutex_t * clientListLock);
    ~ClientManager();
    std::list<ClientData*>* getClientList();
    bool addClient (ClientData client);
    std::list<int> getClientSDList();
    void removeClient(int clientSD);
    void removeClientLogin(int clientSD);
    int getTrainId(int clientSD);
    bool clientIsLogedIn(int clientSD);
    void asignTrainId(int sd, int trainId);
};

