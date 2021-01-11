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
    std::list<ClientData> clientList;
public:
    ClientManager(/* args */);
    ~ClientManager();
    std::list<ClientData>* getClientList();
    bool addClient (ClientData client);
    std::list<int> getClientSDList();
    void removeClient(int clientSD);
};

