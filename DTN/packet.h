#ifndef PACKET_H
#define PACKET_H


/* Maximum size of data chunk that can be sent in a packet */

#define MAX_FRAME_SIZE		1000
#define TYPE_BEACON			1
#define TYPE_DATA			2
#define TYPE_ACK			3
#pragma pack(push, 1)

typedef struct Apacket {
	uint8_t  type;
	uint32_t length;
	char source[4];
	char dest[4];
	uint16_t seq_num;
	uint32_t ttl;
	char data[MAX_FRAME_SIZE];
} packet;

#pragma pack(pop)


#endif