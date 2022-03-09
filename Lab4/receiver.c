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
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<time.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>


int main(int argc, char const *argv[]){

    // for writing into the file
    FILE *fptr;
    fptr = fopen("receiver.txt", "w");

    // for time
    struct timeval stop, start;

    // Socket
    int domain = AF_INET;
    int type = SOCK_DGRAM;     //UDP
    int protocol = 0;

    int server_socket = socket(domain, type, protocol);

    // check for errors
    if(server_socket < 0){
        perror("Server Socket not created!");
        exit(EXIT_FAILURE);
    }

    // BIND
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server)); 
    memset(&client, 0, sizeof(client));

    int port  = atoi(argv[2]); 

    server.sin_family = AF_INET;                // IPv4 
    server.sin_port = htons(port);              // Host Byte order to Network Byte order 
    server.sin_addr.s_addr = INADDR_ANY;        // giving all avalible adresses

    int server_bind = bind(server_socket, (const struct sockaddr *)&server, sizeof(server));

    // check for errors
    if(server_bind < 0){
        perror("Not able to BIND!");
        exit(EXIT_FAILURE);
    }   

    int i=1;
    double pck_prob = atof(argv[3]);

    while(1){   
        // PROCESS
        char buffer[1024];
        unsigned int len = sizeof(client);
        int rcvd = recvfrom(server_socket, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&client, &len);

        buffer[rcvd] = '\0';
        int num;
        // printf(buffer);
        sscanf(&buffer[7], "%d", &num);

        // Part C
        double rnd_val = (1.0*rand())/RAND_MAX;
        if(num == i){
            if(rnd_val<pck_prob){
                // No Ack
                printf("packet%d Droped!\n", i);
                fprintf(fptr, "packet%d Droped!\n", i);
                continue;
            }
            else{
                // SEND
                char msg[1024];
                sprintf(msg, "Acknowledgment:%d", i+1);
                sendto(server_socket, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&client, len);
                printf("Acknowledgment:%d Sent!\n", i+1);
                fprintf(fptr, "Acknowledgment:%d Sent!\n", i+1);
                ++i;
            }
        }
        // Part B
        else{
            // wrong ACK!
            char *msg;
            sprintf(msg, "Acknowledgment:%d", i);
            sendto(server_socket, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&client, len);
            printf("Acknowledgment:%d Sent!\n", i);
            fprintf(fptr, "Acknowledgment:%d Sent!\n", i);
        }
        // save as soon as u get
        fflush( fptr );
    }
    
    fclose(fptr);
    return 0;
}
