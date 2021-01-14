#include "../headers/Request.hpp"
Request::Request(/* args */)
{
}

Request::~Request()
{
}
void Request::setRequestType(int reqType){
    this->reqType=reqType;
}
int Request::getReqestType(){
    return this->reqType;
}
void Request::setClient(int client){
    this->client=client;
}
int Request::getClient(){
    return this->client;
}

void Request::setRequestParameter(char* parameter){
    this->reqParameter=parameter;
}
char* Request::getRequestParameter(){
    return this->reqParameter;
}
void Request::printInfo(){
    printf("[Request Info]Request de la %d ,tipul requestului este %d",this->client,this->reqType);
    if(this->reqType==LOGIN||this->reqType==UPDATE){
        printf(" paramentrul este : %s",this->reqParameter);
    }
    printf("\n");
}