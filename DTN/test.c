#include <stdio.h>
#include "bundle.h"
#include <pthread.h>
#include <string.h>

char listenTo[4];
char * temp;
//void printIP(char * ip) {
//	printf("%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
//}


void * waitForData() {
	char buffer[1024];
	printf("Listening for data from anyone");
	//printIP(listenTo);
	printf("\n");
	buffer[1023] = '\0';
	while (1) {
	char buffer[1024];
	//gets(buffer);
		int a = DTN_datareceive(NULL, buffer, 1023);
		buffer[a] = '\0';
		printf("From %s: %s\n", temp, buffer);
	}
}



int main(int argv, char * args[]) {
system("clear");
temp = args[1];
	//initialize(interface);
	createBundleLayer(args[2]);


	int a,b,c,d;
	sscanf(args[1], "%d.%d.%d.%d", &a, &b, &c, &d);
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
		sscanf(buffer, "%d.%d.%d.%d", &a, &b, &c, &d);
		IP[0] = a;
		IP[1] = b;
		IP[2] = c;
		IP[3] = d;
		gets(buffer);
		printf("Sending to ");
		printIP(IP);
		printf(": %s\n", buffer);
		DTN_datasend(IP, buffer, strlen(buffer));
	}
}
