/**
 * Manages all sequence number related functions
 */
 #ifndef SEQUENCE_H
#define SEQUENCE_H

#include "packet.h"
#include <stdint.h>

//Lifetime of sequence numbers in seconds, must be greater than lifetime of packet
#define SEQUENCE_TIMEOUT 60

#define KEEP_PACKET 1
#define DELETE_PACKET 0
#define NOT_OLD_PACKET 0
#define OLD_PACKET 1

//Tells whether this is an old packet or if it MAY be new
//Returns OLD_PACKET if it is an old packet, returns NOT_OLD_PACKET if it may be new
int isOld(packet * p);

//Adds the proper sequence number to the given packet
void addSequenceNumber(packet * p);

//Tells whether you should keep the packet p based on the given ACK
//Returns KEEP_PACKET if you should keep, DELETE_PACKET if should delete
int keepPacket(packet * ACK, packet * p);

#endif