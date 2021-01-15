#include "../headers/RequestProcesor.hpp"

RequestProcesor::RequestProcesor(ClientManager * clientManager,InfoTrenuri* infoT)
{
    this->clientManager=clientManager;
    this->infoT=infoT;
}

RequestProcesor::~RequestProcesor()
{
}
RequestProcesor::RequestProcesor(){}

int RequestProcesor::sendResponse(int clientSD,  int status ,char* msg){
    if(write(clientSD,&status,sizeof(int))==-1){
        perror("[SERVER]Eroare la write()\n");
        return -1;
    }
    if(msg!=NULL){
        int len =  strlen(msg)+1;
        if(write(clientSD,&len, sizeof(int))==-1){
            perror("[SERVER]Eroare la write()\n");
            return -1;
        }
        if(write(clientSD,msg,len)==-1){
            perror("[SERVER]Eroare la write()\n");
            return -1;
        }
    }
    
    return 0;
}

void RequestProcesor::processRequest(Request request){
    
    switch (request.getReqestType())
    {
    case LOGIN:{
        if(this->clientManager->clientIsLogedIn(request.getClient())==false){
            
            if(infoT->verify_id(atoi(request.getRequestParameter()))==true){
               
                this->clientManager->asignTrainId(request.getClient(),atoi(request.getRequestParameter()));
                //trimite confirmare la client 
                printf("[Server]Clientul %d s-a logat cu id-ul %s\n",request.getClient(),request.getRequestParameter());
                sendResponse(request.getClient(),SUCCES_NO_MSG,NULL);
            }
            else{
                printf("[Server]Login esuat1 .\n");
                sendResponse(request.getClient(),FAILUARE,NULL);
            }
        }
        else{
            printf("[Server]Login esuat2 .\n");
            sendResponse(request.getClient(),FAILUARE,NULL);
        }
    }break;
    case QUIT:{
        int trId=this->clientManager->getTrainId(request.getClient());
        if(this->clientManager->clientIsLogedIn(request.getClient())==true){
            this->infoT->unlock_id(trId);
            this->clientManager->removeClient(request.getClient());
            printf("[TEST]Client deconectat . \n");
        }
        sendResponse(request.getClient(),SUCCES_NO_MSG,NULL);
        close(request.getClient());
    }break;
    case TODAY:{
        printf("[TEST]today . \n");
        if(this->clientManager->clientIsLogedIn(request.getClient())==true){
            char*msg= infoT->todayTrains();
            sendResponse(request.getClient(),SUCCES,msg);
        }
        else sendResponse(request.getClient(),FAILUARE,NULL);

    }break;
    case PHOUR:{
        printf("[TEST]phour . \n");
        if(this->clientManager->clientIsLogedIn(request.getClient())==true){
            char*msg= infoT->currentHourTrainsLeave();
            sendResponse(request.getClient(),SUCCES,msg);
        }
        else sendResponse(request.getClient(),FAILUARE,NULL); 
    }break;
    case SHOUR:{
        printf("[TEST]shour . \n");
        if(this->clientManager->clientIsLogedIn(request.getClient())==true){
            char*msg= infoT->currentHourTrainsArive();
            sendResponse(request.getClient(),SUCCES,msg);
        }
        else sendResponse(request.getClient(),FAILUARE,NULL); 
    }break;
    case UPDATE:{
        printf("[TEST]update . \n");
        if(this->clientManager->clientIsLogedIn(request.getClient())==true){
            int trId=this->clientManager->getTrainId(request.getClient());
            infoT->updateSosire(trId,atoi(request.getRequestParameter()));
            sendResponse(request.getClient(),SUCCES_NO_MSG,NULL);
        }
        else sendResponse(request.getClient(),FAILUARE,NULL); 
    }break;
    default:
        break;
    }
    
}

