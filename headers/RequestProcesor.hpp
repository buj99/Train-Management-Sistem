#pragma once
#include "Request.hpp"
#include "ClientManager.hpp"
#include "RequestTypes.hpp"
#include "info_trenuri.hpp"

class RequestProcesor
{
private:
    ClientManager * clientManager;
    InfoTrenuri* infoT;
    
public:
    RequestProcesor(ClientManager * clientManager,InfoTrenuri* infoT);
    RequestProcesor();
    ~RequestProcesor();
    int sendResponse(int clientSD,  int status ,char* msg);
    void processRequest(Request request);
};
