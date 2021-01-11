#pragma once
#include "./ClientManager.hpp"
#include <list>
#include <mutex>
class RequestManager
{
private:
    ClientManager * clientManager;
    std::mutex* clientManagerLock;
public:
    RequestManager(/* args */);
    RequestManager(ClientManager* clientManager, std::mutex* clientManagerLock);
    ~RequestManager();
    void start();
};
