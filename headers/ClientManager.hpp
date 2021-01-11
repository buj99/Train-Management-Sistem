#pragma once
#include <stdio.h>
#include "ClientData.hpp"
#include <list>
class ClientManager
{
private:
    std::list<ClientData> clientList;
public:
    ClientManager(/* args */);
    ~ClientManager();
    std::list<ClientData>* getClientList();
    bool addClient (ClientData client);
};

