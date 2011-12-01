#include "bundle.h"
#include "queuestruc.h"
#include "packet.h"
#include "DTN.h"
#include "sequence.h"
#include <stdio.h>
#include <string.h>
#include "dataQueue.h"
#include "sequence.h"
#include "config.h"
#include <unistd.h>

extern config * configuration;
packet * lastPacket = NULL;

packet * deliverPacket(packet * p) {
	packet * ack = NULL;
	int seq = chk_seq(p);
	int storedSeq = getStoredSeqNumber(p)->seqNum;
	if (storedSeq == 0)
		clearSeqNums(p);
	if((storedSeq != 0 && seq == p->seq_num-1) || (storedSeq == 0 && p->seq_num == 1))	{
		add_datapacketnode(p);
		ack = malloc(sizeof(packet));
		memcpy(ack->dest, p->source, 4);
		memcpy(ack->source, p->dest, 4);
		ack->type = TYPE_ACK;
		ack->ttl = configuration->packetLife;
		sprintf(ack->data, "ACK");
		ack->length = sizeof(packet) - MAX_DATA_SIZE + 3;
		ack->seq_num = p->seq_num;
		isOld(ack);
		return ack;
	}
	else
		return NULL;
}



void newPacket(char * dest, char * data, int len) {

	//printf("MY bundle IP IS ");
	//printIP(configuration->IP);
	//printf("\n");
	//if (len == 10)
	//printf("New packet (len: %d) (data %c%c%c%c%c%c%c%c%c%c)\n", len, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
	//else
	//printf("New packet (len: %d) (data %c%c)\n", len, data[0], data[1]);
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, configuration->IP, 4);
	memcpy(p->data, data, len);
	p->ttl = configuration->packetLife;
	p->length = sizeof(packet) - MAX_DATA_SIZE + len;
	addSequenceNumber(p);
	//printf("NEW PACKET SEND seq: %d\n", p->seq_num);
	

	//printf("packet size %d\n", p->length);
	add_packetnode(p);
}


void DTN_datasend(char *destip,char *fulldata, int len) {
	int num;
	int i=0;;
	if (len==0)
		return;
	num=(len/MAX_DATA_SIZE)+1;
	while(i<num) {
		int length = (i==num-1?len-i*MAX_DATA_SIZE:MAX_DATA_SIZE);
		if (length > 0)
			newPacket(destip, &fulldata[MAX_DATA_SIZE*i],length);
		i++;
	}
}


int minm(int a, int b) {
	if(a<b) 
		return a;
	return b;
} 

int DTN_datareceive(char *srcip,char *buffer,int bufferlen) {
	packet * p = NULL;
	
	while(p == NULL) {
		p = getOldestPacket(srcip);
		lastPacket = p;
		if (p == NULL) {
			sleep(configuration->beaconInterval/2000);
		}
	}
	
	memcpy(buffer,p->data,minm(p->length - sizeof(packet) + MAX_DATA_SIZE,bufferlen));

	return minm(p->length - sizeof(packet) + MAX_DATA_SIZE,bufferlen);;
}

packet * getLastPacket() {
	return lastPacket;
}

int createBundleLayer(char * configFile) {
	config * c = getConfiguration(configFile);
	initialize(c);
}

