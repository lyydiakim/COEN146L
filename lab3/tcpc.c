//COEN 146L : Lab3, step 1: TCP client requesting a file transfer from the server.
//Lydia Kim
//Coen 146L Wednesday 5:15
//Date: May 2, 2023
//Title: TCP Client Side
//Description:This file sets up the client side of the tcp connection. It interacts with the tcps file to look
//copy the data in to the destination file.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    //Get from the command line, server IP, src and dst files.
    if (argc != 5){
		printf ("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
		exit(0);
    } 
    //Declare socket file descriptor and buffer
    int sockfd;
    char buf[10];
    
    //Declare server address to accept 
 	struct sockaddr_in my_server;
	my_server.sin_family=AF_INET;
	my_server.sin_port=htons(atoi(argv[2]));
	my_server.sin_addr.s_addr = htonl(INADDR_ANY);

    //Declare host    
    //get hostname
    
    //Open a socket, if successful, returns
	sockfd=socket(AF_INET,SOCK_STREAM,0);    
	
    //Set the server address to send using socket addressing structure
    //Connect to the server
  	connect(sockfd,(struct sockaddr *) &my_server, sizeof(my_server));
	
    //Client sends the name of the file to retrieve from the server
    write(sockfd,argv[3],sizeof(argv[3]));

    //Client begins to write and read from the server
 	int dst;
	dst= open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);	// opens a file for writing; erases old file/creates a new file
	if (dst <0) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", argv[4], errno);
		exit(0);
	}
			
	//bytes is used to ensure how many more characters need to be written to the file
		int bytes=0;
		int sum=0;	

	// reads content of file is loop iterations until end of file
	while((bytes=read(sockfd,buf,sizeof(buf)))>0){

	// writes bytes_read to dst_filename 
		sum++;
		write(dst,buf,bytes);
	}

	printf("%d" ,sum);
	
	// closes dst file pointer
	close(dst);
	

    //Close socket descriptor
    close(sockfd);
    return 0;
}

