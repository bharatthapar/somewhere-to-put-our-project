#include <stdio.h>
#include <stdlib.h>

#include "queuestruc.h"
#include "packet.h"
#include "DTN.h"
#include "sequence.h"

queue *root;
int lock = 1;

//function to add a new node containing the new packet in the main queue structure at the node
void add_packetnode(packet *p1) {
	while(!lock);
	lock = 0;
	int dup=0;
	queue *temp,*last;
	if(root==NULL) {
		root=(struct dataqueue*)malloc(sizeof(struct dataqueue));
		root->p=p1;
		root->next=NULL;
		root->marked = 0;
		root->time_in = time(NULL);
	} else {
		temp=root;
		while(temp!=NULL) {
			if(memcmp((temp->p)->source,p1->source,4)==0 && memcmp((temp->p)->dest,p1->dest,4)==0 && (temp->p)->seq_num==p1->seq_num && p1->type==(temp->p)->type ) {
				dup=1;
				break;
			}
		last=temp;
		temp=temp->next;
		}
		if(dup!=1) {
			last->next=(struct dataqueue*)malloc(sizeof(struct dataqueue));
			last=last->next;
			last->p=p1;
			last->next=NULL;
			last->marked = 0;
			last->time_in = time(NULL);
		}
	}
	lock = 1;
}

void printIP(char * ip) {
	printf("%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
}

//function to display all the packets in the main queue structure at the node
void print_all() {
return;
	while(!lock);
	lock = 0;
	queue * temp = root;
	int i=0;
	int data = 0;
	int ack = 0;
	while(temp!=NULL) {
		if (temp->p->type == TYPE_DATA)
			data++;
		else if (temp->p->type == TYPE_ACK)
			ack++;
		temp=temp->next;
		i++;
	}
	//system("clear");
	printf("Number of data packets present: %d\n", data);
	printf("Number of ACKS present %d\n", ack);
	lock=1;
}

//function to delete the packets from the queue structure that have been set marked as to be deleted 
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
			temp2=temp;
			temp=temp->next;
			free(temp2->p);
			free(temp2);
		} else {
			prev = temp;
			temp=temp->next;
		}
	}
}

//function to run through the queue structure and send all the packets unicast to the IP given by serverip
void send_all(char *serverip) {
	while(!lock);
	lock = 0;
	queue *	temp = root;
	while(temp!=NULL) {
		time_current = time(NULL);
		if((time_current - (temp->time_in)) < temp->p->ttl) {
			temp->p->ttl -=(time_current - temp->time_in);
			temp->time_in = time(NULL);
			sendPacket((temp->p), serverip);
		} else {
			temp->marked = 1;	
		}
		temp=temp->next;
	}
	delete_marked();
	lock = 1;
}

//function to mark a node in the main queue structure to be deleted if the packet therein is to be deleted
void delete_packetnode(packet *p1) {
	while(!lock);
	lock = 0;
	queue *temp;
	int result;
	temp=root;
	while(temp!=NULL) {
		if((keepPacket(p1,temp->p)==DELETE_PACKET)) {
			temp->marked = 1;
		}
		temp = temp->next;
	}
	delete_marked();
	lock = 1;
}



