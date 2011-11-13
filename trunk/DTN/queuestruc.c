#include <stdio.h>
#include <stdlib.h>

#include "queuestruc.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"

queue *root,*head;
int lock = 1;
void add_packetnode(packet *p1) {
	
	while(!lock);
	lock = 0;
	int dup=0;
	queue *node,*head3,*last;
	if(root==NULL) {
		node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
		//printf("allocate %d\n", node);
		node->p=p1;
		node->next=NULL;
		root=node;
		head=root;
	} else {
		head3=root;
		while(head3!=NULL) {

			if(memcmp((head3->p)->source,p1->source,4)==0 && memcmp((head3->p)->dest,p1->dest,4)==0 && (head3->p)->seq_num==p1->seq_num && p1->type==(head3->p)->type ) {
				////printf("IP in duplicate is %d.%d.%d.%d\n\n",p1->source[0],p1->source[1],p1->source[2],p1->source[3]);	
				////printf("%d\n%d\n",p1->type,(head3->p)->type);
				////printf("Seq num is %d\n",p1->seq_num);
				////printf("Duplicate Packet.. won't add it to queue\n");
				dup=1;
				break;
			}
		last=head3;
		head3=head3->next;
		}
		if(dup!=1) {
			node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
			//printf("allocate %d\n", node);
			node->p=p1;
			node->next=NULL;
			last->next=node;
			last=node;
			////printf("added packet to queue\n");
			//if(p1->type == TYPE_ACK)
				//printf("\n\n\n\n\n\nADDED ACK TO QUEUE\n\n\n\n\n\n\n");
		}
	}
	
	//printf("");
	lock = 1;
}

void printIP(char * ip) {
	printf("%d.%d.%d.%d", ip[0]<0?ip[0]+256:ip[0], ip[1]<0?ip[1]+256:ip[1], ip[2]<0?ip[2]+256:ip[2], ip[3]<0?ip[3]+256:ip[3]);
}

void print_all() {
	return;
	while(!lock);
	lock = 0;
	queue *head3;
	int i=0;
	head3=root;
	//printf("Printing packets in the queue:\n");
	while(head3!=NULL) {
		printf("----Packet-----\n");
		printf("type: %d\n",head3->p->type);
		printf("source: ");
		printIP(head3->p->source);
		printf("\n");
		printf("dest: ");
		printIP(head3->p->dest);
		printf("\n");
		printf("sequence: %d",head3->p->seq_num);
		printf(" %s\n",head3->p->data);
		printf("---End Packet---\n");
		
		head3=head3->next;
		i++;
	}
	//printf("Queue ends\n");
	lock=1;
}




void send_all(char *serverip) {
	while(!lock);
	lock = 0;
	queue *head2;

	if(root==NULL) {
		//printf("No packets in queue right now. Aborted send packets operation\n");
		//return -1;
	}

	head2=root;
	while(head2!=NULL) {
		sendPackets((head2->p), serverip);
		head2=head2->next;
	}
	lock = 1;
	//return 0;
}

/*
void send_packetnode(packet *p1,char *serverip)
{
queue *node,*head2;
packet *p2;

if(root==NULL)
{
//printf("No packets in queue right now. Aborted send packet node operation");
}

else
{
head2=root;
while(head2!=NULL)
{
p2=head2->p;
if(p2.type==p1->type && strcmp(p2.source,p1->source)==0 && strcmp(p2.dest,p1->dest)==0 && p2.seq_num==p1->seq_num)
{
sendPackets((head2->p), serverip);
break;
}
}}

}
*/

void delete_packetnode(packet *p1) {
	while(!lock);
	lock = 0;
	queue *node,*head2,*temp,*prev;
	packet *p2;
	int result;
	if(root==NULL) {
		//printf("No packets in queue right now. Aborted delete node operation");
	} else {
		head2=root;
		prev = NULL;
		
		while(head2!=NULL) {
			p2=head2->p;
			if(keepPacket(p1,p2)==DELETE_PACKET) {
				//printf("DELETE!!\n");
				if (prev == NULL) {
					root = head2->next;
				} else {
					prev->next = head2->next;
				}
				temp=head2;
				head2=head2->next;
				free(temp->p);
				//printf("free %d\n", temp);
				free(temp);
				
			} else {
				prev = head2;
				head2=head2->next;
			}
			
				
			
		}
		//if(prev!=NULL)
		//	prev->next = NULL;
		//else
		//	if(root!=NULL)root->next = NULL;
	}
	lock = 1;
}



