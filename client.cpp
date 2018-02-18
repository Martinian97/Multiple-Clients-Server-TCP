#define _BSD_SOURCE
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define BACKLOG 10 // how many pending connections queue will hold

int main(int argc, char* argv[])	{
	sockaddr_in my_addr;
	int sockfd;
	
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1],(in_addr*)&(my_addr.sin_addr));
	memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct
	
	sockfd = socket (PF_INET, SOCK_STREAM, 0); 	
	connect(sockfd, (struct sockaddr *)&my_addr,sizeof(struct sockaddr));
	
	char welcome[1024];
	recv(sockfd,welcome,1024,0);
	cout<<"Server Message : "<<welcome<<endl;		
	
	char buffer[1024];
	char reply[1024];
	while(true)	{
		cout<<"Enter message for server : ";
		cin.getline(buffer,1024);
		send(sockfd, buffer, 1024, 0);
		recv(sockfd,reply,1024,0);
		cout<<"Server Message : "<<reply<<endl;
		if(reply[0]=='B' and reply[1]=='y' and reply[2]=='e')	{
			close(sockfd);
			cout<<"Closing Connection."<<endl;
			break;
		}
	}	
	return 0;
}
