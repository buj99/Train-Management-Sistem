#pragma once
#include "./ClientManager.hpp"
#include <list>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>
class RequestManager
{
private:
    enum requestType{login, quit , today,phour,shour,update};
    ClientManager * clientManager;
    std::mutex* clientManagerLock;
    std::list<int> clientSDList;
public:
    RequestManager(/* args */);
    RequestManager(ClientManager* clientManager, std::mutex* clientManagerLock);
    ~RequestManager();
    void start();
};
