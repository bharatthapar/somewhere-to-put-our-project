#ifndef QUEUESTRUC_H
#define QUEUESTRUC_H

#include "packet.h"
#include <inttypes.h>
#include<time.h>
time_t time_current;

typedef struct dataqueue {
	packet *p;
	struct dataqueue *next;
	time_t time_in;
	int marked;
} queue;


void add_packetnode(struct Apacket *packet);
void send_all(char *ip);
void send_packetnode(struct Apacket *packet,char*ip);
void delete_packetnode(struct Apacket *packet);
void print_all();
void printIP(char * ip);

#endif
