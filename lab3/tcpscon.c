//COEN 146L : Lab3, step 4 and 5
//Lydia Kim
//Coen 146L Wednesday 5:15
//Date: May 2, 2023
//Title: TCP Concurrent Server
//Description:This file is a concurrent server that can handle multiple threads.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/stat.h>
#include <pthread.h>


//Define the number of clients/threads that can be served
#define N 100
int threadCount = 0;
pthread_t clients[N]; //declaring N threads


 //Declare socket and connection file descriptors.
	int sockfd;
	int cfd;

 //Declare receiving and sending buffers of size 10 bytes
	char buf[10];

 //Declare server address to which to bind for receiving messages and client address to fill in sending address
	struct sockaddr_in server;
	struct sockaddr_in clienAddr;

//Connection handler (thread function) for servicing client requests for file transfer
void* connectionHandler(void* sock){

   //declare buffer holding the name of the file from client
   	char filename[10];
	int src;
	cfd=*(int*)sock;

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
   	
	//receive name of the file from the client
	bzero(filename,sizeof(filename));
	read(cfd,filename,255);

   //open file and send to client
     src=open(filename,O_RDONLY);

   //read file and send to connection descriptor
   	int b=0;
	while((b=read(src,buf,sizeof(buf)))>0){
		write(cfd,buf,b);
	}
   printf("File transfer complete\n");
   
   //close file
 	close(src); 
  
   //Close connection descriptor
	close(cfd);
   pthread_exit(0);
}


int main(int argc, char *argv[]){
 //Get from the command line, server IP, src and dst files.
 if (argc != 2)
 {
	printf ("Usage: %s <port #> \n",argv[0]);
	exit(0);
 } 


 //Open a TCP socket, if successful, returns a descriptor
 	sockfd=socket(AF_INET,SOCK_STREAM,0);


 //Setup the server address to bind using socket addressing structure
 	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=htonl(INADDR_ANY);

 //bind IP address and port for server endpoint socket
 	bind(sockfd,(struct sockaddr *) &server,sizeof(server));

 // Server listening to the socket endpoint, and can queue 5 client requests
 printf("Server listening/waiting for client at port %d\n",atoi(argv[1]));
	int backlog=5;
	listen(sockfd,backlog);
 
 while (1)
 {
   //Server accepts the connection and call the connection handler
 	int connfd=accept(sockfd,NULL,NULL);  
   if(pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) &connfd) < 0){
      perror("Unable to create a thread");
      exit(0);
   }
   else 
      printf("Thread %d has been created to service client request\n",++threadCount);
 }

int i=0;
 for( i = 0; i < threadCount; i++)
 {
      pthread_join(clients[i], NULL);
 }
 return 0;
}
