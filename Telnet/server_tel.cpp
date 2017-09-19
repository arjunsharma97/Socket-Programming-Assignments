#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
using namespace std;
#define PORT 8080

int main(){
	int socket_id, newsockfd, valueread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer_ftp [10000] = {0};
	int option = 1;
	char message[10000];
	bool set_socket_option;
	int bind_status;
	int listen_status;
	string sum1;

	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id < 0){
		cout << "Error while setting socket... \n";
	}
	set_socket_option = setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, option);
	if(set_socket_option){
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr("127.0.0.1");
		address.sin_port = htons( PORT );
	}else{
		cout << "Error while setting socket... \n";
		exit(EXIT_FAILURE);
	}
	bind_status = bind(socket_id, (sockaddr *)&address, sizeof(address));
	if(bind_status == -1){
		cout << "Error while binding... \n";
		exit(EXIT_FAILURE);
	}
	listen_status = listen(socket_id, 3);
	if(listen_status == -1){
		cout << "Error while listening... \n";
		exit(EXIT_FAILURE);
	}
	newsockfd = accept(socket_id, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if(newsockfd <0){
		cout << "Error while accepting new socket... \n";
		exit(EXIT_FAILURE);
	}
	while(1){
		memset(&message, '\0', sizeof(message));
		read(newsockfd,message,10000);
		cout<<message<<endl;
		if(!strcmp(message,"add")){
			int flag = 1;
			int sum=0;
			while(flag==1){
				memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
				read(newsockfd,buffer_ftp,sizeof(buffer_ftp));
				if(atoi(buffer_ftp)==-1){
					flag=0;
					memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
					sprintf(buffer_ftp, "%d", sum);
					write(newsockfd,buffer_ftp,sizeof(buffer_ftp));
				}
				else{
					sum = sum + atoi(buffer_ftp);
				}
			}

		}
		else if(!strcmp(message,"multiply")){
			int flag = 1;
			int sum = 1;
			while(flag==1){
				memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
				read(newsockfd,buffer_ftp,sizeof(buffer_ftp));
				if(atoi(buffer_ftp)==-1){
					flag=0;
					memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
					sprintf(buffer_ftp, "%d", sum);
					write(newsockfd,buffer_ftp,sizeof(buffer_ftp));
				}
				else{
					sum = sum * atoi(buffer_ftp);
				}
			}
		}
		else if(!strcmp(message,"ls")){
			system("ls>.temp1");
			ifstream test_file (".temp1");
			int length_doc;
			if(test_file){		
				test_file.seekg (0, test_file.end);
				int length_doc = test_file.tellg();
				test_file.seekg (0, test_file.beg);

				char buffer_file_whole[length_doc];
				memset(&buffer_file_whole, '\0', sizeof(buffer_file_whole));
				
				test_file.read(buffer_file_whole, length_doc);

				write(newsockfd, buffer_file_whole, length_doc);			
				
				test_file.close();
				remove(".temp1");
			}
		}
		else if(!strcmp(message,"pwd")){
			system("pwd>.temp1");
			ifstream test_file (".temp1");
			int length_doc;
			if(test_file){		
				test_file.seekg (0, test_file.end);
				int length_doc = test_file.tellg();
				test_file.seekg (0, test_file.beg);

				char buffer_file_whole[length_doc];
				memset(&buffer_file_whole, '\0', sizeof(buffer_file_whole));
				
				test_file.read(buffer_file_whole, length_doc);

				write(newsockfd, buffer_file_whole, length_doc);			
				
				test_file.close();
				remove(".temp1");
			}
		}
		else if(!strcmp(message,"cd")){
			int status=0;
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			read(newsockfd,buffer_ftp,sizeof(buffer_ftp));
			status = chdir(buffer_ftp);
			memset(&buffer_ftp, '\0', sizeof(buffer_ftp));
			sprintf(buffer_ftp, "%d", status);
			write(newsockfd,buffer_ftp,sizeof(buffer_ftp));

		}
		else if(!strcmp(message,"exit")){
			break;
		}
		else{
			//cout<<message<<endl;
			strcat(message,">.temp1 \n");
			//cout<<message<<endl;
			system(message);
			ifstream test_file(".temp1");
			int length_doc;
			//cout<<"Done"<<endl;
			if(test_file){
				//cout<<"Done again"<<endl;		
				test_file.seekg (0, test_file.end);
				int length_doc = test_file.tellg();
				int is_empty=0;
				test_file.seekg (0, test_file.beg);
				if(length_doc==0){
					length_doc = 5;
					is_empty=1;					
				}
				char buffer_file_whole[length_doc];
				if(is_empty==0){
				memset(&buffer_file_whole, '\0', sizeof(buffer_file_whole));
				test_file.read(buffer_file_whole, length_doc);
				}else{
					strncpy(buffer_file_whole, "empty", sizeof(buffer_file_whole));

				}
				//cout<<"Length of doc is "<<length_doc<<endl;
				//cout<<"Writing..."<<endl;
				//cout<<"Buffer file whole: "<<buffer_file_whole<<endl;
				write(newsockfd, buffer_file_whole, sizeof(buffer_file_whole));			
				//cout<<"Closing..."<<endl;
				test_file.close();
				//cout<<"Removing..."<<endl;
				remove(".temp1");
			}
			//string str(message);
			/*string mess(message,message + sizeof message/sizeof message[0]);
			//strcat(message,">temp.txt");
			cout<<"hi";
			cout<<message;
			system(str);
			ifstream test_file ("temp.txt");
			int length_doc;
			if(test_file){		
				test_file.seekg (0, test_file.end);
				int length_doc = test_file.tellg();
				test_file.seekg (0, test_file.beg);

				char buffer_file_whole[length_doc];
				
				test_file.read(buffer_file_whole, length_doc);

				write(newsockfd, buffer_file_whole, length_doc);			
				
				test_file.close();
				remove("temp.txt");
			}*/


		}	
	}
	return 0;
}