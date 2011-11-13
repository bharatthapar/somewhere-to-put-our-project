#include "bundle.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"

packet * deliverPacket(packet * p) {
	packet * ack = malloc(sizeof(packet));
	memcpy(ack->dest, p->source, 4);
	memcpy(ack->source, p->dest, 4);
	ack->type = TYPE_ACK;
	sprintf(ack->data, "ACK");
	ack->length = sizeof(packet) - MAX_FRAME_SIZE + 3;
	ack->seq_num = p->seq_num;
	printf("Seq num %d\n", p->seq_num);
	return ack;
}

void newPacket(char * source, char * dest) {
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, source, 4);
	sprintf(p->data, "TEST PACKET");
	p->length = sizeof(packet) - MAX_FRAME_SIZE + 11;
	addSequenceNumber(p);
	printf("Seq num %d\n", p->seq_num);
	add_packetnode(p);
}
