#pragma once
#include <stdio.h>
#include "sys/time.h"
class ClientData
{
private:
    int sd;
    int trainId=-1;
    
public:
    ClientData(/* args */);
    ClientData(int clientSD);
    int getSD();
    void setTrainId(int tainId);
    int getTrainId();
    ~ClientData();
};