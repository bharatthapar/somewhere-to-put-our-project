#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include<time.h>
time_t time_current;

//dataQueue structure at each node containing all the packets for each flow receieved by the node as the final destination 
typedef struct data_Queue {
	packet *p;
	struct data_Queue *next;
	int marked;
} data_Queue;

int chk_seq(packet *p);
packet * getOldestPacket(char * srcip);
void add_datapacketnode(packet *p1);
void clearSeqNums(packet * p1);
#endif
