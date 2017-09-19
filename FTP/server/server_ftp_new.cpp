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
	char choice[1];
	bool set_socket_option;
	int bind_status;
	int listen_status;

	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id < 0){
		cout << "Error while setting socket... \n";
	}
	set_socket_option = setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, option);
	if(set_socket_option){
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr("127.0.0.1");
		address.sin_port = htons(PORT);
	}else{
		cout << "Error while setting socket... \n";
	}
	bind_status = bind(socket_id, (sockaddr *)&address, sizeof(address));
	if(bind_status == -1){
		cout << "Error while binding... \n";
	}
	listen_status = listen(socket_id, 3);
	if(listen_status == -1){
		cout << "Error while listening... \n";
	}
	newsockfd = accept(socket_id, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if(newsockfd <0){
		cout << "Error while accepting new socket... \n";
	}
	read(newsockfd,choice,1);
	if(choice[0]=='1'){
		cout<<"File Download requested by client"<<endl;
		ifstream test_file ("server_download.txt", ifstream::binary);
		int length_doc;
		if(test_file){		
			test_file.seekg (0, test_file.end);
			length_doc = test_file.tellg();
			test_file.seekg (0, test_file.beg);
			char filebuf[1024];
			while(1){
				memset(&filebuf, '\0', sizeof(filebuf));
				test_file.read(filebuf, 1024);
				write(newsockfd, filebuf, 1024);
				if(test_file.eof()){
					//memset(&filebuf, '\0', sizeof(filebuf));
					//sprintf(filebuf, "eoffile");
					//write(newsockfd, filebuf, 1024);
					break;
				}			
			}
			test_file.close();
		}

		cout << "File successfully downloaded. "<<endl;	
	}
	else if(choice[0]=='2'){
		cout<<"File upload requested by client"<<endl;
		ofstream test_file;
		test_file.open("server_upload.txt", ofstream::out | ofstream::trunc);
		test_file.close();
		test_file.open("server_upload.txt",ios_base::app);
		char filebuf[1024];
		while(1){
			memset(&filebuf, '\0', sizeof(buffer_ftp));
			valueread = read(newsockfd, buffer_ftp, 1024);
			if(valueread<= 0){
				cout<<"Ending"<<endl;
				break;	
			}
			test_file << buffer_ftp;
		}
		test_file.close();
	}
	return 0;
}