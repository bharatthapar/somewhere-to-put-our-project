/**
 * Manages packets that are to be forwarded to others
 */

#include <stdio.h>
#include <stdlib.h>

#include "forwardQueue.h"
#include "packet.h"
#include "DTN.h"
#include "sequence.h"

//Stores the current time
time_t time_current;

//The root node of the queue
queue *root;

//Used for mutex locks
int lock = 1;

//Adds a new packet to the forward queue if it doesn't already exist
void add_packetnode(packet * p1) {
	//Obtain the lock
	while(!lock);
	lock = 0;
	
	int dup=0;
	queue * temp, * last;
	
	//Add the packet if the queue is empty
	if(root == NULL) {
		root = (struct dataqueue*)malloc(sizeof(struct dataqueue));
		root->p = p1;
		root->next = NULL;
		root->marked = 0;
		root->time_in = time(NULL);
	} else {
	
		//Check to make sure the packet is not a duplicate
		temp = root;
		while(temp != NULL) {
			if(memcmp((temp->p)->source,p1->source,4) == 0 && memcmp((temp->p)->dest,p1->dest,4) == 0 && (temp->p)->seq_num == p1->seq_num && p1->type == (temp->p)->type ) {
				dup = 1;
				break;
			}
			last = temp;
			temp = temp->next;
		}
		
		//Add the packet if it is not a duplicate
		if(dup!=1) {
			last->next = (struct dataqueue*)malloc(sizeof(struct dataqueue));
			last = last->next;
			last->p = p1;
			last->next = NULL;
			last->marked = 0;
			last->time_in = time(NULL);
		}
	}
	
	//Release the lock
	lock = 1;
}

//Prints an IP stored in byte format
void printIP(char * ip) {
	printf("%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
}

//Displays all the packets in the queue
void print_all() {
	//To turn off this feature
	return;
	
	//Obtain the lock
	while(!lock);
	lock = 0;
	
	queue * temp = root;
	int i = 0;
	int data = 0;
	int ack = 0;
	while(temp != NULL) {
		if (temp->p->type == TYPE_DATA)
			data++;
		else if (temp->p->type == TYPE_ACK)
			ack++;
		temp = temp->next;
		i++;
	}
	//system("clear");
	printf("Number of data packets present: %d\n", data);
	printf("Number of ACKS present %d\n", ack);
	
	//Release the lock
	lock=1;
}

//Delete the packets from the queue that have been marked to be deleted 
void delete_marked() {
	queue * prev = NULL;
	queue * temp2;
	queue * temp = root;
	while(temp!=NULL) {
		if(temp->marked == 1) {
			if (prev == NULL) {
				root = temp->next;
			} else {
				prev->next = temp->next;
			}
			temp2 = temp;
			temp = temp->next;
			free(temp2->p);
			free(temp2);
		} else {
			prev = temp;
			temp = temp->next;
		}
	}
}

//Sends all the packets unicast to the IP given by serverip
void send_all(char * serverip) {
	//Obtain the lock
	while(!lock);
	lock = 0;
	
	queue *	temp = root;
	while(temp!=NULL) {
		time_current = time(NULL);
		
		//Send the packet and update the TTL if it has not timed out
		if((time_current - (temp->time_in)) < temp->p->ttl) {
			temp->p->ttl -=(time_current - temp->time_in);
			temp->time_in = time(NULL);
			sendPacket((temp->p), serverip);
			
		//The packet has timeout out, mark it for deletion
		} else {
			temp->marked = 1;	
		}
		temp=temp->next;
	}
	delete_marked();
	
	//Release the lock
	lock = 1;
}


//Marks a node in the queue to be deleted if the packet therein is to be deleted
void delete_packetnode(packet * p1) {

	//Obtain the lock
	while(!lock);
	lock = 0;
	
	queue *temp;
	int result;
	temp=root;
	
	//Find and mark packets to delete
	while(temp!=NULL) {
		if((keepPacket(p1,temp->p)==DELETE_PACKET)) {
			temp->marked = 1;
		}
		temp = temp->next;
	}
	
	//Delete the packets
	delete_marked();
	
	//Release the lock
	lock = 1;
}
