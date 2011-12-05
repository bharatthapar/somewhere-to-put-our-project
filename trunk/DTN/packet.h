#ifndef PACKET_H
#define PACKET_H

#include <inttypes.h>

//Maximum amount of data that a packet can contain
#define MAX_DATA_SIZE		1400

#define TYPE_BEACON			1
#define TYPE_DATA			2
#define TYPE_ACK			3

#pragma pack(push, 1)

//Structure to store a complete packet
typedef struct Apacket {
	uint8_t  type;
	uint32_t length;
	char source[4];
	char dest[4];
	uint32_t seq_num;
	uint32_t ttl;
	char data[MAX_DATA_SIZE];
} packet;

#pragma pack(pop)
#endif
