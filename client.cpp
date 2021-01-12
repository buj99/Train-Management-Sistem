
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

/* codul de eroare returnat de anumite apeluri */
extern int errno;

enum requestType{login, quit , today,phour,shour,update,nocomand};
requestType request;
requestType setRequest(char* msg);
void makeRequest(requestType request , char* parameter);
/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
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
    case login:{
      scanf("%s",msg);
      makeRequest(request,msg);
      }
      break;
    case quit:
      makeRequest(request,NULL);
    break;
    case today:
      makeRequest(request,NULL);
      break;
    case phour:
      makeRequest(request,NULL);
      break;
    case shour:
      makeRequest(request,NULL);
      break;
    case update:{
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
  // len= strlen(msg)+1;
  // printf("[Client]Am primit comanda : %s de lungime %d\n",msg,len);
  // write(sd,&len,sizeof(int));
  // write(sd,msg,len);
  
  close (sd);
}
requestType setRequest(char* msg){

  if(strcmp(msg,"today")==0)return today;
  if(strcmp(msg,"login")==0)return login;
  if(strcmp(msg,"quit")==0)return quit;
  if(strcmp(msg,"phour")==0)return phour;
  if(strcmp(msg,"shour")==0)return shour;
  if(strcmp(msg,"update")==0)return update;
  fflush(stdin);
  return nocomand;
}

void makeRequest(requestType request , char* parameter){
  write(sd,(int)request,sizeof(int));
  if(parameter!=NULL){
    write(sd,(int)(strlen(parameter)+1),sizeof(int));
    write(sd,parameter,strlen(parameter)+1);
    
  }
  printf("[TEST]msg:%s\n",msg);
}