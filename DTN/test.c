#include <stdio.h>
#include "bundle.h"
#include <pthread.h>
#include <string.h>

char listenTo[4];

//void printIP(char * ip) {
//	printf("%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
//}


void * waitForData() {
	char buffer[1024];
	printf("Listening for data from: ");
	printIP(listenTo);
	printf("\n");
	buffer[1023] = '\0';
	while (1) {
		DTN_datareceive(listenTo, buffer, 1023);
		printf("Data received: %s\n", buffer);
	}
}



int main(int argv, char * args[]) {


	//initialize(interface);
	initialize(args[1]);






	int a,b,c,d;
	sscanf(args[2], "%d.%d.%d.%d", &a, &b, &c, &d);
	listenTo[0] = a;
	listenTo[1] = b;
	listenTo[2] = c;
	listenTo[3] = d;
	char IP[4];
	pthread_t sender,receiver;
	pthread_create(&receiver,NULL,waitForData,NULL);
	char buffer[1024];
	while (1) {
		gets(buffer);
		sscanf(args[1], "%d.%d.%d.%d", &a, &b, &c, &d);
		IP[0] = a;
		IP[0] = b;
		IP[0] = c;
		IP[0] = d;
		gets(buffer);
		printf("Sending to ");
		printIP(IP);
		printf(": %s\n", buffer);
		DTN_datasend(IP, buffer, strlen(buffer));
	}
}
