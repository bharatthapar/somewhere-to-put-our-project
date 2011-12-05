#ifndef BUNDLE_H
#define BUDLE_H

#include "packet.h"

//Creates an ACK packet and adds the packet to the delivery queue
packet * deliverPacket(packet * p);

//Creates a new data Packet 
void newPacket(char * dest, char * data, int len);

//DTN_datareceive: api functonality similar to socket recvfrom() function to recieve data from the bundle layer
int DTN_datareceive(char * srcip, char * buffer, int bufferlen);

//DTN_datasend: api functonality similar to socket sendto() function to send data to the bundle layer 
void DTN_datasend(char * destip, char * fulldata, int len);

//Creates and initializes the bundle layer based on the configuration file specified by configFile
int createBundleLayer(char * configFile);

//Returns the last packet who's data was delivered
//Used if information such as the source address is needed by the user
packet * getLastPacket();

#endif
