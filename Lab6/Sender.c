#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/time.h>

#define MAXBUFFERSIZE 256

int main(int argc, char const *argv[]){

  char *ip = "127.0.0.1";

  // Socket
  int domain = AF_INET;
  int type = SOCK_STREAM;      //TCP
  int p = 0;

  int sockfd = socket(domain, type, p);

  // Handling errors
  if(sockfd >=0){
    printf("Sender: Socket Created Successfully!\n");
  }
  else{
    perror("Sender: Error in creating Socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serveraddr, clientaddr;

  int port  = atoi(argv[1]);

  serveraddr.sin_family = AF_INET;                     // IPv4 
  serveraddr.sin_port = port;                         // Host Byte order to Network Byte order 
  serveraddr.sin_addr.s_addr = inet_addr(ip);        // giving all avalible adresses

  int sin_size = sizeof(struct sockaddr_in);


  // Check protocol demand
  char protocol[256];
  if(argv[2]="reno"){
    strcpy(protocol, "reno");
    int _len = strlen(protocol);
    setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, protocol, sizeof(protocol)); 
  }
  else{
    strcpy(protocol, "cubic");
    int _len = strlen(protocol);
    setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, protocol, sizeof(protocol));
  }

  // Binding
  int server_bind = bind(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

  // check for errors
  if(server_bind < 0){
      perror("Sender: Not able to BIND!");
      exit(EXIT_FAILURE);
  }
  else{
    printf("Sender: Bind Successfully!\n");
  }

  // Listening
  if(listen(sockfd, 5) == 0){
    printf("Sender: Listen Successfully!\n");
  }
  else{
    perror("Sender: Error in listening!\n");
    exit(EXIT_FAILURE);
  }

 
  socklen_t addr_size = sizeof(struct sockaddr_in);
  int peer_socket = accept(sockfd, (struct sockaddr *)&clientaddr, &addr_size);

  if(peer_socket == -1){
    printf("Sender: Error in accepting\n");
    exit(EXIT_FAILURE);
  }


  int n;
  FILE *fp;
  char *filename = "send.txt";
  char buffer[MAXBUFFERSIZE]={0};
  char c[10000];

  fp = fopen(filename, "r");

  while(fgets(buffer, MAXBUFFERSIZE, fp) != NULL){
    write(peer_socket, buffer, strlen(buffer));
    bzero(buffer, MAXBUFFERSIZE);
  }

  fclose(fp);
  close(sockfd);

  return 0;
}