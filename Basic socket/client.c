#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
  
int main()
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char message[250];
    char buffer[1024] = {0};
    char exitsta;
    int i=1;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\n Address error \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Connection Failed \n");
        return -1;
    }
    printf("Welcome client! \n", sock);
    while(i=1){
        memset(&message, '\0', sizeof(message));
        memset(&buffer, '\0', sizeof(buffer));
        printf("Enter the message you want to send to the server : ");
        gets(message);
        //scanf("%s",message);
        send(sock,message, strlen(message), 0);
        printf("Message sent to server\n");
        valread = read( sock , buffer, 1024);
        printf("Message received from server: %s\n",buffer );
        printf("Do you want to exit?(Y/n) => ");
        scanf(" %c",&exitsta);
        printf("Exit status is %c \n", exitsta);
        if(exitsta=='Y'){
            return 0;
        }
        scanf("%c",&exitsta);
    }
}
/*while(i=1){
        printf("Enter the message you want to send to the server: ");
        scanf("%s",buffer);
        send(sockfd,buffer, strlen(buffer), 0);
        printf("Message sent to server\n");
        n = read( sockfd , buffer, 1024);
        printf("Message received from server: %s\n",buffer );
        printf("Do you want to exit?(Y/n) => ");
        scanf("%s",buffer);
        if(strcmp(buffer, "Y") == 0){
            return 0;
        }
    }*/