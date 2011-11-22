#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include<time.h>
time_t time_current;

typedef struct data_Queue {
	packet *p;
	struct data_Queue *next;
	int marked;
} data_Queue;

int chk_seq(packet *p);
packet * getOldestPacket(char * srcip);
void add_datapacketnode(packet *p1);

#endif
