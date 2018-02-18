#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BACKLOG 10
using namespace std;

int main(int argc, char* argv[])	{
	sockaddr_in my_addr;
	int sockfd,x;
	unsigned int sin_size;
	
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_port = htons(atoi(argv[1]));
	inet_aton("127.0.0.1",(in_addr*)&(my_addr.sin_addr));
	memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct
	
	sockfd = socket (PF_INET, SOCK_STREAM, 0); 
	
	bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
	if(x==-1)	{
		cout<<"Binding unsuccessful."<<endl;
		return 0;
	}
	cout<<"Successfully Binded."<<endl;	

	listen(sockfd, BACKLOG);
	cout<<"Server is Listening..."<<endl;
	sin_size = sizeof(struct sockaddr_in);

	int pid,newf,counter=0	;
	char welcome[1024];
	strcpy(welcome, "Good Morning! Welcome to Mr. Ishan's server.");
		    
	while(true)	{
		newf = accept(sockfd, (struct sockaddr *)&my_addr, &sin_size);
		send(newf, welcome, 1024, 0);
		cout<<"Connected to new client socket number "<<sockfd<<endl;
        if ((pid = fork()) == -1)	{
            close(newf);
            continue;
        } 	
        else if(pid > 0)	{
            close(newf);
            counter++;
            continue;
        }
        else if(pid == 0)	{
            char buffer[100];
            char reply[100];
            counter++;
            
            while(true)	{
				recv(newf, buffer, 1024, 0);
				cout<<"Message from "<<sockfd<<" and client"<<counter<<" : "<<buffer<<endl;
				if(buffer[0]=='B' and buffer[1]=='y' and buffer[2]=='e')	{
					cout<<"Replied to "<<sockfd<<" and client"<<counter<<" : "<<buffer<<endl;
					send(newf, buffer, 1024, 0);	
					close(newf);
					break;
				}	
				for(int i=0;i<1024;i++)	{
					reply[i]=toupper(buffer[i]);
				}	
				cout<<"Replied to "<<sockfd<<" and client"<<counter<<" : "<<reply<<endl;
				send(newf, reply, 1024, 0);	
			}
            
            close(newf);
            break;
		}
	}
}
