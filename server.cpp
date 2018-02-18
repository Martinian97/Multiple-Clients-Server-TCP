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
	int sockfd,x,y;
	unsigned int sin_size;
	
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_port = htons(atoi(argv[1]));
	inet_aton("127.0.0.1",(in_addr*)&(my_addr.sin_addr));
	memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct
	
	sockfd = socket (PF_INET, SOCK_STREAM, 0); 
	cout<<"Connected to client socket number "<<sockfd<<endl;
	
	x=bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
	if(x==-1)	{
		cout<<"Binding unsuccessful."<<endl;
		return 0;
	}
	cout<<"Successfully Binded."<<endl;	
	
	listen(sockfd, BACKLOG);
	cout<<"Server is Listening..."<<endl;
	int newsockfd=accept(sockfd,  (struct sockaddr *)&my_addr, &sin_size);
	sin_size = sizeof(struct sockaddr_in);
	char welcome[1024];
	strcpy(welcome, "Good Morning! Welcome to Mr. Ishan's server.");
	send(newsockfd, welcome, 1024, 0);	
	
	char buffer[1024];
	char reply[1024];
	
	while(true)	{
		recv(newsockfd, buffer, 1024, 0);
		cout<<"Message from "<<sockfd<<" : "<<buffer<<endl;
		if(buffer[0]=='B' and buffer[1]=='y' and buffer[2]=='e')	{
			cout<<"Replied to "<<sockfd<<" : "<<buffer<<endl;
			send(newsockfd, buffer, 1024, 0);	
			close(newsockfd);
			break;
		}	
		for(int i=0;i<1024;i++)	{
			reply[i]=toupper(buffer[i]);
		}	
		cout<<"Replied to "<<sockfd<<" : "<<reply<<endl;
		send(newsockfd, reply, 1024, 0);	
		}	
	
	
	return 0;
}
