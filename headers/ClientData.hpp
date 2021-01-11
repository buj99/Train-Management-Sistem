#pragma once
#include <stdio.h>
#include "sys/time.h"
class ClientData
{
private:
    int sd;
    
public:
    ClientData(/* args */);
    ClientData(int clientSD);
    int getSD();
    ~ClientData();
};