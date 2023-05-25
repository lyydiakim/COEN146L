//COEN 146L : Lab3, step 1:  TCP server that accepts a client connection for file transfer.
//Lydia Kim
//Coen 146L Wednesday 5:15
//Date: May 2, 2023
//Title: TCP Server Side
//Description:This file sets up the server side of the tcp connection.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
//Declare socket file descriptor.

int sockfd;

//Declare receiving and sending buffers of size 10 bytes

char buf[10];

//Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in server;
struct sockaddr_in clienAddr;

//Connection handler for servicing client request for file transfer
void* connectionHandler(void* sock){

   //declare buffer holding the name of the file from client
    char filename[10];
    int src; 

   //get the connection descriptor
   int c = *(int*) sock;

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
	   
	//receive name of the file from the client
	bzero(filename, sizeof(filename));
	read(c,filename,255);

   //open file and send to client
     	src=open(filename,O_RDONLY);
	printf("filename: %s", filename);
	int b=0;

   //read file and send to connection descriptor
	while((b=read(src, buf, sizeof(buf)))>0){
	//	sum++;
		write(c,buf,b);	
	}
//	printf("%d",sum);
   printf("File transfer complete\n");
   
   //close file
    close(src);
    close(c);
   
   return 0;
}


int main(int argc, char *argv[]){
 //Get from the command line, server IP, src and dst files.
if (argc != 2){
	printf ("Usage: %s <port #> \n",argv[0]);
	exit(0);
 }

 //Open a TCP socket, if successful, returns a descriptor
	sockfd=socket(AF_INET,SOCK_STREAM,0);

 //Setup the server address to bind using socket addressing structure
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);		

 //bind IP address and port for server endpoint socket 
	bind(sockfd,(struct sockaddr *)&server, sizeof(server));	

 // Server listening to the socket endpoint, and can queue 5 client requests
	printf("Server listening/waiting for client at port %d \n",atoi(argv[1]));
	int backlog=5;
	listen(sockfd,backlog);

 //Server accepts the connection and call the connection handler
	int new_socket=accept(sockfd,NULL,NULL);
	connectionHandler((void*)&new_socket);	
	
 //close socket descriptor
	close(sockfd);

 return 0;
}
