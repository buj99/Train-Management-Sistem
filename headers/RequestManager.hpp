#pragma once
#include "./ClientManager.hpp"
#include "./Request.hpp"
#include <list>
#include <deque>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
class RequestManager
{
private:
    ClientManager * clientManager;
    std::mutex* clientManagerLock;
    std::list<int> clientSDList;
    std::deque <Request * > *requestQue;
    //std::mutex* requestQueLock;
    pthread_mutex_t* requestQueLock;
    pthread_cond_t* gotRequest;
    int checkIfIsConected(int sd);
    Request * decodeRequest(int sd);
public:
    RequestManager(/* args */);
    RequestManager(ClientManager* clientManager, std::mutex* clientManagerLock,std::deque <Request*>* requestQue ,pthread_mutex_t* requestQueLock ,pthread_cond_t *gotRequest);
    ~RequestManager();
    void start();
    int reciveMesage(int sd);
    
};
