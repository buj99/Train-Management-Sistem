#include "../headers/info_trenuri.hpp"

InfoTrenuri::InfoTrenuri()
{
}

InfoTrenuri::~InfoTrenuri()
{
}

int  InfoTrenuri::initiate(char* xml_path){

    res= doc.load_file(xml_path);
    root=doc.first_child();
    printf("%s\n",res.description());
    if(res)return 1 ; 
    return 0;
}
void InfoTrenuri::print_trains(){
    
    for(xml_node train : root.children()){
        printf("Da\n");
        for(xml_attribute atr: train.attributes()){
            printf("%s : %s\n",atr.name(),atr.value());
        }
    }
}
bool InfoTrenuri::verify_id(int id){
        for(xml_node tren : root.children()){
            
            if(atoi(tren.attribute("id").value())==id){
                if(atoi(tren.attribute("conenctat").value())==0){
                    tren.attribute("conenctat").set_value("1");
                    return true;
                }
            }
        }
        return false;
    }

void InfoTrenuri::unlock_id(int id){
    for(xml_node tren : root.children()){
        if(atoi(tren.attribute("id").value())==id){
            tren.attribute("conenctat").set_value("0");
        }
    }
}

char* InfoTrenuri::todayTrains(){
    char*msg =new char();
    char p1[]="Trenul : ";
    char p2[]=" pleaca la ora ";
    char p3[]=":";
    char p4[]="\n";
    //*********************************
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    //*********************************
    //trebuie pus intr o functie separata 
    for(xml_node tren : root.children()){
        bool merge=false;
        if(atoi(tren.attribute("eDay").value())==1){
            merge=true;
        }
        else if(atoi(tren.attribute("zp").value())==timeinfo->tm_mday){
            merge=true;
        }
        if(merge){
            char* temp_msg=new char();
            temp_msg=composeRespons(temp_msg,p1);
            //adauga id : 
            char* id_tren=strdup(tren.attribute("id").value());
            temp_msg=composeRespons(temp_msg,id_tren);
            free(id_tren);
            //adauga "pleaca la ora "
            temp_msg=composeRespons(temp_msg,p2);
            //adauga ora
            char* ora_plecare=strdup(tren.attribute("hp").value());
            temp_msg=composeRespons(temp_msg,ora_plecare);
            free(ora_plecare);
            //adauga ":"
            temp_msg=composeRespons(temp_msg,p3);
            //adauga minutele
            char* minute_plecare=strdup(tren.attribute("mp").value());
            temp_msg=composeRespons(temp_msg,minute_plecare);
            free(minute_plecare);
            //adauga newline
            temp_msg=composeRespons(temp_msg,p4);
            //adauga la mesajul final;
            msg=composeRespons(msg,temp_msg);
            free(temp_msg);
        }
    }
    
    return msg;
}

char* InfoTrenuri::currentHourTrainsLeave(){
    char* msg = new char();
    char p1[]="Trenurile care pleaca in urmatoarea ora sunt :";
    char p2[]=" ";
    bool merge ;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    //adaug o ora 
    int day,hour,minute;
    plus_hour(timeinfo,59,&day,&hour,&minute);
    //incep sa compun rapsunsul
    msg= composeRespons(msg,p1);
    for(xml_node tren : this->root.children()){
        merge=false;
        char* zi_plecare=strdup(tren.attribute("zp").value());
        char* ora_plecare=strdup(tren.attribute("hp").value());
        char* minut_plecare=strdup(tren.attribute("mp").value());
        char* eday= strdup(tren.attribute("eDay").value());
        int* eDay=new int();
        *eDay = atoi(eday);
        free(eday);
        int* tren_plecare=(int*)malloc(sizeof(int)*3);
        tren_plecare[0]=atoi(zi_plecare);
        tren_plecare[1]=atoi(ora_plecare);
        tren_plecare[2]=atoi(minut_plecare);
        free(zi_plecare);
        free(ora_plecare);
        free(minut_plecare);
        if(
            (tren_plecare[2]>=timeinfo->tm_min&&tren_plecare[1]==timeinfo->tm_hour&&(tren_plecare[0]==timeinfo->tm_mday||*eDay==1))||
            (tren_plecare[2]<=minute&&tren_plecare[1]==hour&&(tren_plecare[0]==day||*eDay==1))
        ){
            merge=true;
        }
        if(merge){
            char*id_tren=strdup(tren.attribute("id").value());
            
            msg=composeRespons(msg,id_tren);
            msg=composeRespons(msg,p2);
            free(id_tren);
        }
        
    }
    return msg;
}

char* InfoTrenuri::currentHourTrainsArive(){
    char* msg = new char();
    char p1[]="Trenurile care ajung in urmatoarea ora sunt :";
    char p2[]=" ";
    bool merge ;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    //adaug o ora 
    int day,hour,minute;
    plus_hour(timeinfo,59,&day,&hour,&minute);
    //incep sa compun rapsunsul
    msg= composeRespons(msg,p1);
    for(xml_node tren : this->root.children()){
        merge=false;
        //conditie data actuala 
        //la sosire verific daca ora de sosire este egala cu ora actuala si minutul sosire 
        //este mai mic sau egal decat minutul actual si ziua de sosire este egala cu ziua actuala 
        //conditie data peste o ora 
        //sau verific daca new_hour este egala cu ora de sosire si new_minute este mai mare sau egal 
        //cu minutul actual si ziua de sosire este egala cu noua zi
        char* zi_sosire=strdup(tren.attribute("zs").value());
        char* ora_sosire=strdup(tren.attribute("hs").value());
        char* minut_sosire=strdup(tren.attribute("ms").value());
        char* eday= strdup(tren.attribute("eDay").value());
        int* eDay=new int();
        *eDay = atoi(eday);
        free(eday);
        int* tren_sosire=(int*)malloc(sizeof(int)*3);
        tren_sosire[0]=atoi(zi_sosire);
        tren_sosire[1]=atoi(ora_sosire);
        tren_sosire[2]=atoi(minut_sosire);
        free(zi_sosire);
        free(ora_sosire);
        free(minut_sosire);
        if(
            (tren_sosire[2]>=timeinfo->tm_min&&tren_sosire[1]==timeinfo->tm_hour&&(tren_sosire[0]==timeinfo->tm_mday||*eDay==1))||
            (tren_sosire[2]<=minute&&tren_sosire[1]==hour&&(tren_sosire[0]==day||*eDay==1))
        ){
            merge=true;
        }
        if(merge){
            char*id_tren=strdup(tren.attribute("id").value());
            
            msg=composeRespons(msg,id_tren);
            msg=composeRespons(msg,p2);
            free(id_tren);
        }
        free(eDay);
        free(tren_sosire);
    }
    return msg;
}

char* InfoTrenuri::composeRespons(char* curent_char , char* concat_char){
    curent_char=(char*)realloc(curent_char,strlen(curent_char)+strlen(concat_char));
    strcat(curent_char,concat_char);
    
    return curent_char;
}

void InfoTrenuri::plus_hour(struct tm* date ,int min_add , int* new_day, int* new_hour , int* new_minute){
    *new_minute=date->tm_min+min_add;
    if(*new_minute>=59){
        *new_minute-=59;
        *new_hour=date->tm_hour+1;
        *new_day=date->tm_mday;
        if(*new_hour==24){
            *new_hour=0;
            *new_day++;
            if(*new_day==32){
               *new_day=1;
            }
        
        }
    }
    else
    {
        *new_hour=date->tm_hour;
        *new_day=date->tm_mday;
    }
    
}

void InfoTrenuri::updateSosire(int trainID ,int intarziere){}