// Name: Lydia Kim
// Date: 5/25/23
// Title : Lab 2 - Step 4
// Description : This uses the labs given values to calculate different values of 
// round trip time delending on the type of persistence and parallel nature
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {  // check correct usage
		fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
		exit(1);
	}

	//Declare and values to n (n connections), np (np parralel connections), RTT0, RTT1, RTT2, RTTHTTP, RTTDNS, ..
	int RTT0 = 3; //local dns cache
	int RTT1 = 20; //2nd dns server
	int RTT2 = 26;  // 3rd dns server
	int RTTHTTP = 47;  //between local host and web server



	//non persistent: 1 object sent over a single tcp connection
	int n  = strtol(argv[1],NULL,10);
	int RTTDNS = (RTT0+RTT1+RTT2+(2*RTTHTTP));
	int np = 6/n;


	if ( 6 % n != 0)
		np++;

	printf("One object: %d msec\n", RTTDNS); 
	printf("Non-Persistent 6 objects: %d msec\n", (((2*6*RTTHTTP) + (2*RTTHTTP) + (RTT0) + (RTT1) + (RTT2)))); 


	RTTDNS = (RTT0+RTT1+RTT2+((np+2)*RTTHTTP));
	printf("%d parallel connection - Persistent: %d msec\n",n, RTTDNS) ; //425 msec
	RTTDNS = (RTT0+RTT1+RTT2+(RTTHTTP * (2*np + 2)));	
	printf("%d parallel connection - Non-Persistent: %d msec\n", n ,RTTDNS );  //707 msec

	return 0;
}