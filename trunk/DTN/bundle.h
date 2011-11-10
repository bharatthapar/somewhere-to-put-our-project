#ifndef BUNDLE_H
#define BUDLE_H

#include "packet.h"

packet * deliverPacket(packet * p);
void newPacket(char * source, char * dest);

#endif
