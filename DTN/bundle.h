#ifndef BUNDLE_H
#define BUDLE_H

#include "packet.h"

packet * deliverPacket(packet * p);
void newPacket(char * dest, char * data, int len);
int DTN_datareceive(char *srcip,char *buffer,int bufferlen);
void DTN_datasend(char *destip,char *fulldata, int len);

#endif

