#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fstream>

#define PORT 8080

using namespace std;

int main(){	
	struct sockaddr_in address;
	int socket_id;
	int valueread;
	struct sockaddr_in serv_addr;
	int addrlen = sizeof(address);
	char message[10000] = {0};
	char buffer_ftp[10000] = {0};

	int socket_status;
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id == -1){
		cout << "Error while setting socket... \n";
		return -1;
	} 
	memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int connect_status = connect(socket_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(connect_status == -1){
		cout << "Error while connecting from client side... \n"; 
		return -1;
	}
	cout<<endl;
	while(1){
		cout<<"Enter the command you want to run: ";
		gets(message);
		write(socket_id, message, 10000);
		if(!strcmp(message,"add")){
			int flag = 1;
			cout<<endl<<"Enter the numbers to be added (-1 to get answer): "<<endl;
			while(flag==1){
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			cin>>buffer_ftp;
			write(socket_id,buffer_ftp,sizeof(buffer_ftp));
			if(atoi(buffer_ftp)==-1){
				flag=0;
				cout<<endl;
				cout<<"------Exited-------"<<endl;
				read(socket_id,buffer_ftp,sizeof(buffer_ftp));
				cout<<"The answer is "<<buffer_ftp<<endl;
				cin.ignore();
			}
			}
		}
		else if(!strcmp(message,"multiply")){
			int flag = 1;
			cout<<endl<<"Enter the numbers to be multiplied (-1 to get answer): "<<endl;
			while(flag==1){
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			cin>>buffer_ftp;
			write(socket_id,buffer_ftp,sizeof(buffer_ftp));
			if(atoi(buffer_ftp)==-1){
				flag=0;
				cout<<endl;
				cout<<"------Exited-------"<<endl;
				read(socket_id,buffer_ftp,sizeof(buffer_ftp));
				cout<<"The answer is "<<buffer_ftp<<endl;
				cin.ignore();
			}
			}
		}
		else if(!strcmp(message,"ls")){
			cout<<endl;
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			read(socket_id, buffer_ftp, 10000);
			cout<<buffer_ftp<<endl;
		}
		else if(!strcmp(message,"pwd")){
			cout<<endl;
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			read(socket_id, buffer_ftp, 10000);
			cout<<buffer_ftp<<endl;
		}
		else if(!strcmp(message,"cd")){
			cout<<endl;
			cout<<"Enter the directory: ";
			gets(message);
			write(socket_id,message,sizeof(message));
			read(socket_id, buffer_ftp, 10000);
			if(atoi(buffer_ftp)==0){
				cout<<"Directory changed successfully"<<endl;
			}
			else{
				cout<<"Error while changing directory"<<endl;
			}
		}
		else if(!strcmp(message,"exit")){
			break;
		}
		else{
			//cout<<"In else"<<endl;
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			read(socket_id, buffer_ftp, 10000);
			cout<<buffer_ftp<<endl;
			//cout<<"Invalid command"<<endl;
			/*read(socket_id, buffer_ftp, 10000);
			cout<<buffer_ftp<<endl;*/
		}
		cout<<endl;
	}
	close(socket_id); 
	return 0;
}