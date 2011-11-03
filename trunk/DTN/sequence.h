#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "packet.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct seqHolder {
	char address[4];
	uint32_t ACKSeqNum;
	uint32_t dataSeqNum;
	uint32_t timeout;
	struct seqHolder * next;
} sequence;

sequence * first;
sequence * last;

void * allocate(size_t size) {
	void * out = malloc(size);
	if (out == NULL) {
		fprintf(stderr, "Memory error, aborting.\n");
		exit(-1);
	}
	return out;
}

sequence * newSeqNumber(packet * p) {
	sequence * out = allocate(sizeof(sequence));
	if (p->type == TYPE_DATA) {
		out->address[0] = p->source[0];
		out->address[1] = p->source[1];
		out->address[2] = p->source[2];
		out->address[3] = p->source[3];
		
		out->dataSeqNum = p->seq_num - 1; 
	} else if (p->type == TYPE_ACK) {
		out->address[0] = p->dest[0];
		out->address[1] = p->dest[1];
		out->address[2] = p->dest[2];
		out->address[3] = p->dest[3];
		
		out->ACKSeqNum = p->seq_num - 1; 	
	}
	out->timeout = 0;
	out->next = NULL;
	return out;
}

sequence * getStoredSeqNumber(packet * p) {
	//No records in list, create a new one
	if (first == NULL) {
		first = newSeqNumber(p);
		last = first;
		return first;
	} else {
		sequence * temp;
		//Search the list for a record
		for(temp = first; temp != NULL; temp = temp->next) {
			if ((p->type == TYPE_DATA && (uint32_t)*temp->address == (uint32_t)*p->source) ||
					(p->type == TYPE_ACK && (uint32_t)*temp->address == (uint32_t)*p->dest)) {
				return temp;
			}
		}
		//No record found, create a new one
		last->next = newSeqNumber(p);
		last = last->next;
		return last;
	}
}

int checkDataQueue(packet * p) {
	sequence * s;
	
	s = getStoredSeqNumber(p);

	return -7;
}

int checkACKQueue(packet * P) {
	return -5;
}

int isNew(packet * p) {
	switch (p->type) {
		case TYPE_BEACON:
			return 0;
		case TYPE_ACK:
			return checkACKQueue(p);
		case TYPE_DATA:
			return checkDataQueue(p);
		default:
			return 0;
	}
	return 0;
}

void addSequenceNumberToPacket(packet * p) {
	
}

int keepPacket(packet * ACK, packet * p) {

}


#endif