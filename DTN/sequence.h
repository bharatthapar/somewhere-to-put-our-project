#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "packet.h"
#include <stdint.h>

typedef struct seqHolder {
	char address[4];
	uint32_t ACKSeqNum;
	uint32_t dataSeqNum;
	uint32_t timeout;
	struct seqHolder * next;
} sequence;

//Tells whether this is a new packet or if it has been seen before
//Returns 1 for new, 0 for old
int isNew(packet * p);

//Adds the proper sequence number to the given packet
void addSequenceNumberToPacket(packet * p);

//Tells whether you should keep the packet p based on the given ACK
//Returns 1 if you keep, 0 if you delete
int keepPacket(packet * ACK, packet * p);

#endif