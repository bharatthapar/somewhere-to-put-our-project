#ifndef DELIVERYQUEUE_H
#define DELIVERYQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include <time.h>

//Structure to packets for delivery at the node in a linked list
typedef struct data_Queue {
	packet * p;
	struct data_Queue * next;
	int marked;
} data_Queue;

//Returns the largest sequence number packet received for a particular flow in the queue
int chk_seq(packet * p);

//Gets the oldest packet in the queue for the correspondng flow from srcip
packet * getOldestPacket(char * srcip);

//Adds a new packet containing to the queue
void add_datapacketnode(packet * p1);

//Clears sequence numbers for a flow specified by the given packet 
void clearSeqNums(packet * p1);

#endif
