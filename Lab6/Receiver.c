#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include<sys/time.h>

#define MAXBUFFERSIZE 256

int main(int argc, char const *argv[]){

  char *ip = "127.0.0.1";

  // Socket parameters
  int domain = AF_INET;
  int type = SOCK_STREAM;      //TCP
  int p = 0;

  int sockfd = socket(domain, type, p);

  // Handling errors
  if(sockfd >=0){
    printf("Receiver: Socket Created Successfully!\n");
  }
  else{
    perror("Receiver: Error in creating Socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serveraddr;

  // taking port from the argument
  int port  = atoi(argv[1]);

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;                     // IPv4 
  serveraddr.sin_port = port;                         // Host Byte order to Network Byte order 
  serveraddr.sin_addr.s_addr = inet_addr(ip);        // giving all avalible adresses


  int sin_size = sizeof(struct sockaddr_in);

  // Connect
  if((connect(sockfd, (struct sockaddr *)&serveraddr, sin_size))==0){
    printf("Receiver: Connected Successfully!\n");
  }
  else{
    printf("Receiver: ERROR! Connecting\n");
    exit(EXIT_FAILURE);
  }


  FILE *fp = fopen("recv.txt", "w+");

  if(fp == NULL){
    printf("Receiver: Error opening file!\n");
    exit(EXIT_FAILURE);
  }


  char buffer[MAXBUFFERSIZE+1];

  struct timeval stop, start;
  gettimeofday(&start, NULL);
  

  int n;
  bzero(buffer, MAXBUFFERSIZE);
  while((n=read(sockfd, buffer, MAXBUFFERSIZE)) >0){
    buffer[n]='\0';
    fwrite(buffer, 1, n, fp);
    fflush(fp);
    bzero(buffer, MAXBUFFERSIZE);
  }
  
  gettimeofday(&stop, NULL);
  float diff_t = (stop.tv_sec - start.tv_sec)*1000000 + stop.tv_usec - start.tv_usec;
  // printf("Receiver: Took %f ms to transfer the data!\n", diff_t);
  diff_t = diff_t/1000000.0;

  float throughput = 40000000/diff_t;
  FILE *f = fopen("Throughput.txt", "a");
  fprintf(f, "%s %s %s %f\n", argv[4], argv[3], argv[2], throughput);
  fflush(f);

  fclose(f);
  fclose(fp);
  close(sockfd);

  return 0;
}