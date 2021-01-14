#pragma once
#include <stdio.h>
#include "./RequestTypes.hpp"

class Request
{

private:
    int reqType;
    int client ;
    char* reqParameter;

public:
    Request(/* args */);
    ~Request();
    void setRequestType(int reqType);
    int getReqestType();
    void setClient(int client);
    int getClient();
    void setRequestParameter(char* parameter);
    char* getRequestParameter();
    void printInfo();
};


