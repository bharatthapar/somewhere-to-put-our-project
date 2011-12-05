#ifndef FORWARDQUEUE_H
#define FORWARDQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include <time.h>

//Structure to support storing packets in a linked list
typedef struct dataqueue {
	packet * p;
	struct dataqueue * next;
	time_t time_in;
	int marked;
} queue;

//Adds a new packet to the forward queue if it doesn't already exist
void add_packetnode(struct Apacket * packet);

//Sends all the packets unicast to the IP given by serverip
void send_all(char * ip);

//Marks a node in the queue to be deleted if the packet therein is to be deleted
void delete_packetnode(struct Apacket * packet);

//Displays all the packets in the queue
void print_all();

//Prints an IP stored in byte format
void printIP(char * ip);

#endif
