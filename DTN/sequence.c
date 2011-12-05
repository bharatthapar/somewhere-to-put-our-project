/**
 * Manages all sequence number related functions
 */
 
#include "packet.h"
#include "sequence.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct localNum {
	//The destination
	char dest[4];
	//The current sequence number for the destination
	uint32_t num;
	//For linked list
	struct localNum * next;
	//When this sequence number is no longer valid
	time_t timeout;
} destSeq;

//Head of the linked list for others' sequence numbers
sequence * first;
//Tail of the linked list for others' sequence numbers
sequence * last;
//Head of the linked list for local sequence numbers
destSeq * localFirst;
//Tail of the linked list for local sequence numbers
destSeq * localLast;

//Allocates memory
void * allocate(size_t size) {
	void * out = malloc(size);
	if (out == NULL) {
		fprintf(stderr, "Memory error, aborting.\n");
		exit(-1);
	}
	return out;
}

//Used to create a new record for a locally generated flow
destSeq * newDestSeq(packet * p) {
	destSeq * out = allocate(sizeof(destSeq));
	memcpy(out->dest, p->dest, 4);
	out->num = 0;
	out->timeout = p->ttl + time(NULL);
	out->next = NULL;
	return out;
}

//Gets the last used sequence number for the locally generated flow of the given packet
destSeq * getLocalSeqNumber(packet * p) {
	//Nothing in list, must create new
	if (localFirst == NULL) {
		localFirst = newDestSeq(p);
		localLast = localFirst;
		localFirst->num++;
		return localFirst;
	} else {
		destSeq * temp;
		time_t tm = time(NULL);
		for(temp = localFirst; temp != NULL; temp = temp->next) {
		
			//Check for timeout
			if (tm > temp->timeout)
				temp->num = 0;
		
			if(memcmp(temp->dest, p->dest, 4) == 0) {
				temp->num++;
				return temp;
			}
		}
		temp = newDestSeq(p);
		localLast->next = temp;
		localLast = temp;
		temp->num++;
		return temp;
	}

}

//Called the first time a flow is encountered to store a new sequence number for it
sequence * newSeqNumber(packet * p) {
	sequence * out = allocate(sizeof(sequence));
	if (p->type == TYPE_DATA) {
		memcpy(out->source, p->source, 4);
		memcpy(out->dest, p->dest, 4);

		out->seqNum = 0;
	} else if (p->type == TYPE_ACK) {
		memcpy(out->dest, p->source, 4);
		memcpy(out->source, p->dest, 4);
		
		out->seqNum = p->seq_num - 1; 	
	}
	out->timeout = time(NULL) + p->ttl;
	out->next = NULL;
	return out;
}

//Returns the latest known sequence number for the flow of the given packet
sequence * getStoredSeqNumber(packet * p) {
	//No records in list, create a new one
	if (first == NULL) {
		first = newSeqNumber(p);
		last = first;
		return first;
	} else {
		sequence * temp;
		time_t tm = time(NULL);
		//Search the list for a record
		for(temp = first; temp != NULL; temp = temp->next) {
		
			//Check for timeout
			if (tm > temp->timeout)
				temp->seqNum = 0;
				
			if ((p->type == TYPE_DATA && memcmp(temp->source, p->source, 4) == 0 && memcmp(temp->dest, p->dest, 4) == 0) ||
					(p->type == TYPE_ACK && memcmp(temp->source, p->dest, 4) == 0 && memcmp(temp->dest, p->source, 4) == 0)) {
				return temp;
			}
		}
		//No record found, create a new one
		last->next = newSeqNumber(p);
		last = last->next;
		return last;
	}
}

//Determines whether a data packet is old or possibly new
int checkDataQueue(packet * p) {
	sequence * s;
	s = getStoredSeqNumber(p);
	if (p->seq_num > s->seqNum)
		return NOT_OLD_PACKET;
	else
		return OLD_PACKET;
}

//Determines whether an ACK packet is old or possibly new
int checkACKQueue(packet * p) {
	sequence * s;
	s = getStoredSeqNumber(p);
	if (p->seq_num > s->seqNum) {
		s->seqNum = p->seq_num;
		s->timeout = p->ttl + time(NULL);
		return NOT_OLD_PACKET;
	} else
		return OLD_PACKET;
}

//Tells whether this is an old packet or if it MAY be new
//Returns OLD_PACKET if it is an old packet, returns NOT_OLD_PACKET if it may be new
int isOld(packet * p) {
	switch (p->type) {
		case TYPE_ACK:
			return checkACKQueue(p);
		case TYPE_DATA:
			return checkDataQueue(p);
		default:
			return OLD_PACKET;
	}
	return OLD_PACKET;
}

//Adds the proper sequence number to the given packet (locally generated)
void addSequenceNumber(packet * p) {
	destSeq * temp = getLocalSeqNumber(p);
	temp->timeout = time(NULL) + p->ttl;
	p->seq_num = temp->num;
}

//Tells whether you should keep the packet p based on the given ACK
//Returns KEEP_PACKET if you should keep, DELETE_PACKET if should delete
int keepPacket(packet * ACK, packet * p) {
	if (ACK->type != TYPE_ACK) 
		return KEEP_PACKET;

	if (p->type == TYPE_DATA) {
		//Packet is for a different address pair
		if (!(memcmp(ACK->source, p->dest, 4) == 0 && memcmp(ACK->dest, p->source, 4) == 0)) {
			return KEEP_PACKET;
		} else {
			if (p->seq_num <= ACK->seq_num) {
				return DELETE_PACKET;
			}
			return KEEP_PACKET;
		}
				
	} else if (p->type == TYPE_ACK) {
		//Packet is for a different address pair
		if (!(memcmp(ACK->dest, p->dest, 4) == 0 && memcmp(ACK->source, p->source, 4) == 0)) {
			return KEEP_PACKET;
		} else {
			if (p->seq_num < ACK->seq_num) {
				return DELETE_PACKET;
			}
			return KEEP_PACKET;
		}
	} else {
		return KEEP_PACKET;
	}
}
