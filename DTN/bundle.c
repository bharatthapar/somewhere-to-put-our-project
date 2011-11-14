#include "bundle.h"
#include "queuestruc.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"
#include <stdio.h>

packet * deliverPacket(packet * p) {
	packet * ack = malloc(sizeof(packet));
	memcpy(ack->dest, p->source, 4);
	memcpy(ack->source, p->dest, 4);
	ack->type = TYPE_ACK;
	ack->ttl = 50;
	sprintf(ack->data, "ACK");
	ack->length = sizeof(packet) - MAX_FRAME_SIZE + 3;
	ack->seq_num = p->seq_num;
	isOld(ack);
	//printf("Seq num %d\n", p->seq_num);
	
	p->data[MAX_FRAME_SIZE - 1] = '\0';
	printf("GOT THIS MESSAGE: %s\n", p->data);
	
	return ack;
}

void newPacket(char * dest, char * data, int len) {
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, ipp, 4);
	memcpy(p->data, data, len);
	p->ttl = 50;
	p->length = sizeof(packet) - MAX_FRAME_SIZE + len;
	addSequenceNumber(p);
	//printf("Seq num %d\n", p->seq_num);
	add_packetnode(p);
}

int main(int argv, char * args[]) {
	initialize();
	printf("My IP is ");
	printIP(ipp);
	printf("\n");
	char buf[1000];
	int i;
	char ip[4];
	while(1) {
		gets(buf);
		//printf("\n%s\n", buf);
		
		char * my_ip[5];
			my_ip[0] = (char*)strtok(buf,".");
		i=0;
		while (my_ip[i])
	 	{
			 i++;
	  		 my_ip[i] = strtok (NULL, ".");
			//itoa(my_ip[i],my_ip[i][0],10);
			//printf("%s.....",my_ip[i]);
	  	}
		//printf("%s..%s..%s..%s\n",my_ip[0],my_ip[1],my_ip[2],my_ip[3]);
	ip[0]=atoi(my_ip[0]);
	ip[1]=atoi(my_ip[1]);
	ip[2]=atoi(my_ip[2]);
	ip[3]=atoi(my_ip[3]);
	//printf("IP is %d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
	
	gets(buf);
	printf("Sending %s to ", buf);
	printIP(ip);
	printf("\n");
		newPacket(ip, buf, strlen(buf)+1);
	}
}
