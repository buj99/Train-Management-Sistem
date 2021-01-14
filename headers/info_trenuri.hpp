#pragma once
#include "../xmlparser/pugixml.hpp"
#include <unistd.h>
#include <time.h>
using namespace pugi ;
class InfoTrenuri
{
private:
    pugi::xml_document doc;
    pugi::xml_node root;
    xml_parse_result res ;
    //functions
    char* composeRespons(char* curent_char , char* concat_char);
    void plus_hour(struct tm* date ,int min_add ,int* new_day, int* new_hour , int* new_minute);
    
public:
    InfoTrenuri();
    ~InfoTrenuri();
    int initiate(char* xml_path);
    void print_trains();
    bool verify_id(int id);
    void unlock_id(int id);
    char* todayTrains();
    char* currentHourTrainsLeave();
    char* currentHourTrainsArive();
    void updateSosire(int trainID ,int intarziere);
    
};


