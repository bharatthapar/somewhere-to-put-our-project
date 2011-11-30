#include <stdio.h>
#include "bundle.h"
#include "packet.h"

#include <pthread.h>
#include <string.h>

char listenTo[4];

void printIPString(char * string, char * ip) {
	sprintf(string, "%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
}


void * waitForData() {
	char buffer[1400];
	char ip[20];
	printf("Listening for data from anyone");
	//printIP(listenTo);
	printf("\n");
	buffer[1399] = '\0';
	
	while (1) {
	
	//gets(buffer);
		int a = DTN_datareceive(NULL, buffer, 1023);
		packet * p = getLastPacket();
		printIPString(ip, p->source);
		buffer[a] = '\0';
		printf("From %s: %s\n", ip, buffer);
	}
}



int main(int argv, char * args[]) {
	system("clear");

	//initialize(interface);
	createBundleLayer(args[1]);


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
