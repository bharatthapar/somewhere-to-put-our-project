#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include<time.h>
time_t time_current;

typedef struct data_Queue {
	packet *p;
	struct dataqueue *next;
	int marked;
} data_Queue;

int chk_seq(packet *p);
void add_packetnode(struct Apacket *packet);
void send_all(char *ip);
void send_packetnode(struct Apacket *packet,char*ip);
void delete_packetnode(struct Apacket *packet);
void print_all();

#endif
