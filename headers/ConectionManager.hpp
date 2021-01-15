#pragma once 
#include <pthread.h>
#include <stdio.h>
#include "./ClientManager.hpp"
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <mutex>
class ConectionManager
{
private:
    ClientManager * clientManager;
public:
    ConectionManager(/* args */);
    ConectionManager(ClientManager* clientManager);
    ~ConectionManager();
    int start(int sd);
};

