#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include<inttypes.h>
char ipp[4];
int initialize(); 
void send_beacon();
void data_handler();
void ack_handler();
void *waitForPacket();
void sendPackets(struct Apacket *packet, char *ip);
#endif
