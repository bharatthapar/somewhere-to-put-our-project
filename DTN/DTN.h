#ifndef DTN_H
#define DTN_H

#include "packet.h"


//char ipp[4];
int initialize(); 
void send_beacon();
void data_handler();
void ack_handler();
void *waitForPacket();
void sendPacket(struct Apacket *packet, char *ip);
void setGateway(char * network, char * netMask);
char * getMyIP();
#endif
