#include "bundle.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"

packet * deliverPacket(packet * p) {


}

void newPacket(char * source, char * dest) {
	packet * p = malloc(sizeof(packet));
	memcpy(p->dest, dest, 4);
	memcpy(p->source, source, 4);
	sprintf(p->data, "TEST PACKET");
	p->length = sizeof(packet) - MAX_FRAME_SIZE + 11;
	addSequenceNumber(p);
	add_packetnode(p);
}