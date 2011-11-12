#ifndef QUEUESTRUC_H
#define QUEUESTRUC_H

#include "packet.h"
#include <inttypes.h>

typedef struct dataqueue {
	packet *p;
	struct dataqueue *next;
} queue;


void add_packetnode(struct Apacket *packet);
void send_all(char *ip);
void send_packetnode(struct Apacket *packet,char*ip);
void delete_packetnode(struct Apacket *packet);
void print_all();

#endif
