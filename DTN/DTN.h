#ifndef DTN_H
#define DTN_H

#include "packet.h"

//Function to initialize the self parameters for the connection. These parameters are passed from a config file.
int initialize(); 

//Function called by timer to send beacons periodically
void send_beacon();

//Function called by packetArrival on receiving a data packet
void data_handler();

//Function called by packetArrival on receiving an ACK packet
void ack_handler();

//Function called to by the reciving thread to start the reception
void *waitForPacket();

//Function called to by other functions to send the packets
void sendPacket(struct Apacket * packet, char * ip);

//Function to set the gateway IP and netmask from config file
void setGateway(char * network, char * netMask);

#endif
