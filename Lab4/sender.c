/*******************************************************************************************
CS-252 LAB3
COPYRIGHTS RESERVED

@authors DEEPANSHU (190050032)
         DHAKNE AJAY SOPAN (190050033)
         NITIN KUMAR (190050073)
********************************************************************************************/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

int main(int argc, char const *argv[]){

    // for writing into the file
    FILE *fptr;
    fptr = fopen("sender.txt", "w");
   

    //for time
    struct timeval stop, start;

    // Socket
    int domain = AF_INET;
    int type = SOCK_DGRAM;      //UDP
    int protocol = 0;

    int client_socket = socket(domain, type, protocol);

    // check for errors
    if(client_socket < 0){
        perror("client Socket not created!");
        exit(EXIT_FAILURE);
    }

    // BIND
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server));

    int port = atoi(argv[1]);

    server.sin_family = AF_INET;                // IPv4  
    server.sin_addr.s_addr = INADDR_ANY;        // Take all IPs
    server.sin_port = htons(port);              // Host Byte order to Network Byte order

    int i=1;
    const int re_time= atoi(argv[3]);
    const int num_pck = atoi(argv[4]);

    struct timeval timeout;      
    timeout.tv_sec = re_time;
    timeout.tv_usec = 0;
    setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout));
    
    while(i<=num_pck){  
        // SEND
        char msg[1024];
        sprintf(msg, "Packet:%d", i);

        sendto(client_socket, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(server));
        printf("Packet %d Sent!\n",i);
        fprintf(fptr, "Packet %d Sent!\n", i);

        // See ACK status
        char buffer[1024];
        unsigned int len = sizeof(server);
        int rcvd;
        rcvd = recvfrom(client_socket, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&server, &len);
          
        if(rcvd>0){
            buffer[rcvd] = '\0';
            int num;
            sscanf(&buffer[15], "%d", &num);
           
            if(num == i+1){
                // correct ACK
                i++;
                continue;
            }
            else{
                // Incorrect ACK
                i=num;
                continue;
            }
        }
        else{
            printf("Retransmission Timer Expires!\n");
            fprintf(fptr, "Retransmission Timer Expires!\n");
        }
    }

    close(client_socket);
    fclose(fptr);
    return 0;
}
