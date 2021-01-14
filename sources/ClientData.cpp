#include "../headers/ClientData.hpp"



ClientData::ClientData(/* args */)
{
    printf("Nou CleintData\n");
}
ClientData::ClientData(int clientSD)
{
    this->sd=clientSD;
}

ClientData::~ClientData()
{
}
int ClientData::getSD(){
    return this->sd;
}
int ClientData::getTrainId(){
    return this->trainId;
}
void ClientData::setTrainId(int  trainId){
    this->trainId=trainId;
}