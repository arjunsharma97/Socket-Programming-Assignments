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
	char message[] = "";
	char buffer_ftp[10000] = {0};

	int socket_status;
	char choice[1];
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id == -1){
		cout << "Error while setting socket... \n";
		} 
	memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int connect_status = connect(socket_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(connect_status == -1){
		cout << "Error while connecting from client side... \n"; 
	}
	cout<<"Do you want to download a file(1) or upload a file(2) ?: ";
	cin>>choice;
	cout<<"You chose: "<<choice[0]<<endl;
	write(socket_id, choice, 1);
	if(choice[0]=='1'){
		cout<<"File Download requested"<<endl;
		ofstream test_file;
		test_file.open("client_download.txt");	
		read(socket_id, buffer_ftp, 10000);
		test_file << buffer_ftp;
		test_file.close();

		cout << "File successfully downloaded. "<<endl;	
	}
	else if(choice[0]=='2'){
		cout<<"File Upload requested"<<endl;
		ifstream test_file ("client_upload.txt");
		int length_doc=0;
		if(test_file){
			test_file.seekg (0, test_file.end);
			length_doc = test_file.tellg();
			test_file.seekg (0, test_file.beg);
			char buffer_file_whole[length_doc];
			test_file.read(buffer_file_whole, length_doc);
			int n = write(socket_id, buffer_file_whole, length_doc);
			
			test_file.close();
		}

		cout << "File successfully uploaded."<<endl;
	}
	close(socket_id); 
	return 0;
}