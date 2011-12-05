/**
 * A simple chat program to use over the bundle layer
 *
 * To use type the bundle ip address to send a message to on one line
 * then type the message to send on anouther line
 *
 * usage: ./chat <path to config file>
 */

#include <stdio.h>
#include "bundle.h"
#include "packet.h"

#include <pthread.h>
#include <string.h>

//Converts byte ip adress to printable one
void printIPString(char * string, char * ip) {
	sprintf(string, "%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
}


//Waits to receive data
void * waitForData() {
	char buffer[1400];
	char ip[20];
	printf("Listening for data from anyone");
	printf("\n");
	buffer[1399] = '\0';
	
	//Waits for a packet
	while (1) {
		int a = DTN_datareceive(NULL, buffer, 1023);
		packet * p = getLastPacket();
		printIPString(ip, p->source);
		buffer[a] = '\0';
		//Display who sent the packet and what they sent
		printf("From %s: %s\n", ip, buffer);
	}
}

//Main mesthod and the sending thread
int main(int argv, char * args[]) {
	system("clear");

	//Set up the bundle layer with the specified configuration file
	createBundleLayer(args[1]);
	int a,b,c,d;
	char IP[4];
	pthread_t sender,receiver;
	pthread_create(&receiver,NULL,waitForData,NULL);
	char buffer[1024];
	while (1) {
		gets(buffer);
		
		//Read an address from the command line
		sscanf(buffer, "%d.%d.%d.%d", &a, &b, &c, &d);
		IP[0] = a;
		IP[1] = b;
		IP[2] = c;
		IP[3] = d;
		//scanf("%s", buffer);
		
		//Read the message to send from the command line
		gets(buffer);
		
		printf("Sending to ");
		printIP(IP);
		printf(": %s\n", buffer);
		
		//Send the message
		DTN_datasend(IP, buffer, strlen(buffer));
	}
}
