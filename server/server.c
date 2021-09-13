#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void serviceClient(int);

int main(int argc, char *argv[]){  
  int sd, client, portNumber;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    printf("Got a client\n");

    if(!fork())
      serviceClient(client);

    close(client);
  }
}

void serviceClient(int sd){
	char message[1024] = {0};


  dup2(sd,STDOUT_FILENO);
  dup2(sd,STDERR_FILENO);
  dup2(sd,STDIN_FILENO);
  close(sd);

  write(STDOUT_FILENO, "Connected to server", strlen("Connected to server")+1);

  while(1){

    memset(message,'\0',1024);
		if(!read(STDIN_FILENO, message, 1024)){
			fprintf(stderr, "Bye, my client is dead, wait for a new one\n");
			exit(0);
		}
    else if(strlen(message) == 0)
    {
      fprintf(stderr, "Empty message received");
      continue;
    }
    else if(strcmp(message,"quit") == 0)
    {
      printf("Closing socket\n");
      exit(1);
    }


    char buffer[1024] = {0};
    strncpy(buffer,message,strlen(message));
    memset(message,'\0',1024);

    system(buffer);
	}
}
