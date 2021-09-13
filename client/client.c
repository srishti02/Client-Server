#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]){
  char message[1024] = {0};
  int server, portNumber;
  struct sockaddr_in servAdd;     // server socket address

  if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
    fprintf(stderr, " inet_pton() has failed\n");
    exit(2);
  }


  if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
    fprintf(stderr, "connect() has failed, exiting\n");
    exit(3);
  }

  if (!read(server, message, 1024)){
    fprintf(stderr, "read() error\n");
    exit(3); 
  }

  fprintf(stderr, "Server's messgage: \n%s\n", message);
  while(1)
  {
    // get user input
    memset(message,'\0',1024);
    fprintf(stderr,"Enter command for server to run (Enter $ to quit):\n");
    fgets(message, 1023, stdin);
    if(strncmp(message,"quit",4) == 0){
      printf("Closing socket and exiting!\n");
      write(server, message, strlen(message)+1);
      sleep(1);
      close(server);
      exit(0);
    }

    fprintf(stderr,"Writing to server: %s\n",message);
    // write to server
    write(server, message, strlen(message)+1);
    
    // read from server

    memset(message,'\0',1024);
    sleep(1);
    if (!read(server, message, 1024)){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }

    fprintf(stderr, "Server's messgage: \n%s\n", message);
  }
}


