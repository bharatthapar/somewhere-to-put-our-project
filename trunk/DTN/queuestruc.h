#ifndef QUEUESTRUC_H
#define QUEUESTRUC_H

#include"packet.h"
#include<inttypes.h>

typedef struct dataqueue
{
struct dataqueue *prev;
packet *p;
struct dataqueue *next;
}queue;

queue *root,*head;


void add_packetnode(struct Apacket *packet);
void send_all(char *ip);
void send_packetnode(struct Apacket *packet,char*ip);
void delete_packetnode(struct Apacket *packet);

#endif
