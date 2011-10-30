

/* Maximum size of data chunk that can be sent in a packet */
#define MAX_FRAME_SIZE 		  	1000
#pragma pack(push, 1)

typedef struct packet {
uint8_t  type;
char *srcip;
char *destip;
char  data[MAX_FRAME_SIZE];
uint32_t length;
uint16_t seq_num;
uint32_t ttl;
}datapacket;

#pragma pack(pop)
