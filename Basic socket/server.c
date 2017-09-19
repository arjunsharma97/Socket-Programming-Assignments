#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080
int main()
{
    int sockfd, newsockfd, valread;
    struct sockaddr_in address;
    struct sockaddr_in dest;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char message[250];
    int i=1;
    int cli_id=0;
    char temp;
    int pid;
      
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("Setsockopt failure");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
      
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("Bind failure");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 3) < 0)
    {
        perror("Listen failure");
        exit(EXIT_FAILURE);
    }
    while(i=1){
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("Accept failure");
        exit(EXIT_FAILURE);
    }
        pid = fork();
        if(pid<0){                      
            printf("Unable to create new process for new client\n");
            return 0;                       
        }
        if(pid==0){
            close(sockfd);
            while(i=1){
            memset(&message, '\0', sizeof(message));
            memset(&buffer, '\0', sizeof(buffer));
            cli_id++;
            valread = read( newsockfd , buffer, 1024);
            if(valread<=0){
                return 0;
            }
            printf("Message received from client : %s\n", buffer);  
            printf("Enter the message you want to send to the client: ");
            gets(message);
            send(newsockfd,message, strlen(message), 0);
            printf("Message sent to client\n");
            }
        }
        else{
            close(newsockfd);
        }
    }
    
}