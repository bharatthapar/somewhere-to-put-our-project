#include "bundle.h"
#include "queuestruc.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"
#include <stdio.h>
#include <string.h>
#include "tunnel.h"
int dtn0;
packet * deliverPacket(packet * p) {
	
	packet * ack = malloc(sizeof(packet));
	if(chk_seq(p) == p->seq_num-1)
	{
		add_datapacketnode(p);
		memcpy(ack->dest, p->source, 4);
		memcpy(ack->source, p->dest, 4);
		ack->type = TYPE_ACK;
		ack->ttl = 20;
		sprintf(ack->data, "ACK");
		ack->length = sizeof(packet) - MAX_FRAME_SIZE + 3;
		ack->seq_num = p->seq_num;
		isOld(ack);
		sendPacket(dtn0, p);
		//free packets
		//free(p);
		return ack;
	}
	else
		return NULL;
}



void newPacket(char * dest, char * data, int len) {
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, ipp, 4);
	memcpy(p->data, data, len);
	p->ttl = 20;
	p->length = sizeof(packet) - MAX_FRAME_SIZE + len;
	addSequenceNumber(p);
	add_packetnode(p);
}



int main(int argv, char * args[]) {
	if (! strcmp(args[1],"1")) {
		printf("GATEWAY!!!\n");
		char ip[4];
		char mask[4] = {255,255,255,255};
		char * my_ip[5];
		int i =0, j=0;
		sscanf(args[2], "%d.%d.%d.%d/%d", &ip[0], &ip[1], &ip[2], &ip[3], &i);
		//sscanf(args[3], "%d.%d.%d.%d", &mask[0], &mask[1], &mask[2], &mask[3]);
		
		while (i > 8) {
			mask[j] = 255;
			i = i - 8;
			j++;
		}
		mask[j] = mask[j] << (8-i);
		for (j=j+1; j < 4; j++)
			mask[j] = 0;
		printIP(ip);
		printf("\n");
		printIP(mask);
		printf("\n");		
		setGateway(ip, mask);
		
		//exit(0);
	}

	char *interface = malloc(sizeof(args[1])+1);
	//strcpy(interface,args[1]);
	//printf("Interface is %s\n",interface);
	
	dtn0 = tun_init("dtn0");
	char ipbuf[200] = "ifconfig dtn0 ";
	
	strcat(ipbuf, args[2]);
	strcat(ipbuf, " up");
	
	system(ipbuf);

	//initialize(interface);
	initialize("dtn0");
	printf("My IP is ");
	printIP(ipp);
	printf("\n");
  
	while(1){
		packet * p = getPacket(dtn0);
		if (p != NULL) {
			add_packetnode(p);
		}
	}
}
