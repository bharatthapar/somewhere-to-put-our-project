#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include "packet.h"
#include <inttypes.h>
#include<time.h>
time_t time_current;

typedef struct data_Queue {
	packet *p;
	struct data_Queue *next;
	int marked;
} data_Queue;
data_Queue *root;
int chk_seq(packet *p);

#endif
