/**
 * Manages bundle layer operations such as sending and receiving.
 */
 
#include "bundle.h"
#include "forwardQueue.h"
#include "packet.h"
#include "DTN.h"
#include "sequence.h"
#include "deliveryQueue.h"
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//The configuration file
extern config * configuration;

//The last packet delivered
packet * lastPacket = NULL;

//Creates an ACK packet and adds the packet to the delivery queue
packet * deliverPacket(packet * p) {
	packet * ack = NULL;
	
	//Returns the highest ACKed sequence number for this flow
	int seq = chk_seq(p);
	
	//Gets the stored sequence number for this flow (which will be 0 if the number has timed out or doesn't exist)
	int storedSeq = getStoredSeqNumber(p)->seqNum;
	
	//The sequence number has timed out, reset those of stored packets in the delivery queue
	if (storedSeq == 0)
		clearSeqNums(p);
	
	//ACK the packet and add it to the delivery queue if 
	//the sequence number has not timed out (and exists) and it is the next packet in order
	//or if the sequence number has timeout out (or doesn't exist) and it is the first packet in the flow
	if((storedSeq != 0 && seq == p->seq_num - 1) || (storedSeq == 0 && p->seq_num == 1)) {
	//if(storedSeq == 0 || seq == p->seq_num - 1)	{
		//Add to the delivery queue
		add_datapacketnode(p);
		
		//Create the ACK
		ack = malloc(sizeof(packet));
		memcpy(ack->dest, p->source, 4);
		memcpy(ack->source, p->dest, 4);
		ack->type = TYPE_ACK;
		ack->ttl = configuration->packetLife;
		sprintf(ack->data, "ACK");
		ack->length = sizeof(packet) - MAX_DATA_SIZE + 3;
		ack->seq_num = p->seq_num;
		
		//Let the sequnce handler know about the ACK
		isOld(ack);
		
		return ack;
	}
	else
		return NULL;
}


//Creates a new data Packet 
void newPacket(char * dest, char * data, int len) {
	//Create the packet
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, configuration->IP, 4);
	memcpy(p->data, data, len);
	p->ttl = configuration->packetLife;
	p->length = sizeof(packet) - MAX_DATA_SIZE + len;
	addSequenceNumber(p);

	if (memcmp(dest, configuration->IP, 4) == 0) {
		//Packet is from this node and for this node
		deliverPacket(p);
	} else {
		//Add packet to the forward queue
		add_packetnode(p);
	}
}

//DTN_datasend: api functonality similar to socket sendto() function to send data to the bundle layer 
void DTN_datasend(char * destip, char * fulldata, int len) {
	int num;
	int i = 0;;
	
	//Nothing to send
	if (len == 0)
		return;
	
	//Split the packet into packets no larger than MAX_DATA_SIZE
	num=(len/MAX_DATA_SIZE)+1;
	while(i<num) {
		int length = (i==num-1?len-i*MAX_DATA_SIZE:MAX_DATA_SIZE);
		if (length > 0)
			newPacket(destip, &fulldata[MAX_DATA_SIZE*i],length);
		i++;
	}
}

//Return the minimum of a and b
int minm(int a, int b) {
	if(a < b) 
		return a;
	return b;
} 

//DTN_datareceive: api functonality similar to socket recvfrom() function to recieve data from the bundle layer 
int DTN_datareceive(char * srcip, char * buffer, int bufferlen) {
	packet * p = NULL;
	
	//Wait for a packet to arrive
	while(p == NULL) {
		p = getOldestPacket(srcip);
		lastPacket = p;
		if (p == NULL) {
			sleep(configuration->beaconInterval / 2000);
		}
	}
	
	//Copy the packet into the user supplied buffer
	memcpy(buffer,p->data,minm(p->length - sizeof(packet) + MAX_DATA_SIZE,bufferlen));

	//Return the length of the data
	return minm(p->length - sizeof(packet) + MAX_DATA_SIZE,bufferlen);;
}

//Returns the last packet who's data was delivered
//Used if information such as the source address is needed by the user
packet * getLastPacket() {
	return lastPacket;
}

//Creates and initializes the bundle layer based on the configuration file specified by configFile
int createBundleLayer(char * configFile) {
	//Fill the config structure
	config * c = getConfiguration(configFile);
	
	//Initialize the bundle layer
	initialize(c);
}
