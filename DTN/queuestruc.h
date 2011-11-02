#ifndef QUEUESTRUC_H
#define QUEUESTRUC_H

#include"packet.h"
#include<inttypes.h>

typedef struct dataqueue
{
packet p;
struct dataqueue *next;
}queue;

//typedef struct dataqueue queue;
queue *root,*head;

#endif
