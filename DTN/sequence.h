/**
 * Manages all sequence number related functions
 */
 #ifndef SEQUENCE_H
#define SEQUENCE_H

#include "packet.h"
#include <stdint.h>
#include <time.h>

#define KEEP_PACKET 1
#define DELETE_PACKET 0
#define NOT_OLD_PACKET 0
#define OLD_PACKET 1

//Structure to store sequence numbers in a linked list
typedef struct seqHolder {
	//Source and destination (based on data packet)
	char source[4];
	char dest[4];
	//The most recent sequence number seen (in an ACK)
	uint32_t seqNum;
	//When this sequence number is no longer valid
	time_t timeout;
	//For linked list
	struct seqHolder * next;
} sequence;


//Tells whether this is an old packet or if it MAY be new
//Returns OLD_PACKET if it is an old packet, returns NOT_OLD_PACKET if it may be new
int isOld(packet * p);

//Adds the proper sequence number to the given packet
void addSequenceNumber(packet * p);

//Tells whether you should keep the packet p based on the given ACK
//Returns KEEP_PACKET if you should keep, DELETE_PACKET if should delete
int keepPacket(packet * ACK, packet * p);

//Returns the latest known sequence number for the flow of the given packet
sequence * getStoredSeqNumber(packet * p);

#endif
