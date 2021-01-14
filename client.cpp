
/* cliTCPIt.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include "./headers/RequestTypes.hpp"

/* codul de eroare returnat de anumite apeluri */
extern int errno;

int request;
int setRequest(char* msg);
void makeRequest(int request , char* parameter);
/* portul de conectare la server*/
int port;
int sd;			// descriptorul de socket
int main (int argc, char *argv[])
{
  
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0;
  char buf[10];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  /*start conv */
  char* msg=(char*)malloc(sizeof(char)*10);
  int len;
  while(true){
    printf("introduecti o comanda:");
    
    scanf("%s",msg);
    request=setRequest(msg);
    switch (request)
    {
    case LOGIN:{
      scanf("%s",msg);
      makeRequest(request,msg);
      }
      break;
    case QUIT:
      makeRequest(request,NULL);
    break;
    case TODAY:
      makeRequest(request,NULL);
      break;
    case PHOUR:
      makeRequest(request,NULL);
      break;
    case SHOUR:
      makeRequest(request,NULL);
      break;
    case UPDATE:{
      scanf("%s",msg);
      makeRequest(request,msg);
    }
      break;
    default:{
      printf("[Client]Comanda invalida\n");
    }
      break;
    }
  }

  
  close (sd);
}
int setRequest(char* msg){

  if(strcmp(msg,"today")==0)return TODAY;
  if(strcmp(msg,"login")==0)return LOGIN;
  if(strcmp(msg,"quit")==0)return QUIT;
  if(strcmp(msg,"phour")==0)return PHOUR;
  if(strcmp(msg,"shour")==0)return SHOUR;
  if(strcmp(msg,"update")==0)return UPDATE;
  fflush(stdin);
  return -1;
}

void makeRequest(int request , char* parameter){
  if(write(sd,&request,sizeof(int))!=-1){
    printf("[TEST]Am trimis request %d ",request);
  }
  if(request==LOGIN||request==UPDATE){
    int len = strlen(parameter)+1;
    write(sd,(void*)&len,sizeof(len));
    write(sd,parameter,len);
    printf("cu parametrul \" %s \"  de lungime %d",parameter,len);
  }
  printf("\n");
}