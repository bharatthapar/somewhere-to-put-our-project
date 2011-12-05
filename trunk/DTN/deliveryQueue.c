/**
 * Handles packets that should be delivered to this node
 */

#include "deliveryQueue.h"

//The root of the delivery queue
struct data_Queue * roots = NULL;

//Used for mutex locks
int lock1 = 1;

//Prints all the packets in the queue
void print_all1() {

	//Obtain the lcok
	while(!lock1);
	lock1 = 0;
	
	data_Queue *head3;
	int i = 0;
	head3 = roots;
	int data = 0;
	int ack = 0;
	//printf("Printing packets in the queue:\n");
	while(head3 != NULL) {
		//printf("----Packet-----\n");
		//printf("type: %d\n",head3->p->type);
		//printf("source: ");
		//printIP(head3->p->source);
		//printf("\n");
		//printf("dest: ");
		//printIP(head3->p->dest);
		//printf("\n");
		//printf("sequence: %d",head3->p->seq_num);
		//printf(" %s\n",head3->p->data);
		//printf("---End Packet---\n");
		if (head3->p->type == TYPE_DATA)
			data++;
		else if (head3->p->type == TYPE_ACK)
			ack++;
		
		head3 = head3->next;
		i++;
	}
	system("clear");
	//printf("My IP is ");
	//printIP(ipp);
	printf("\n");
	printf("Number of data packets present: %d\n", data);
	printf("Number of ACKS present %d\n", ack);
	//printf("Queue ends\n");
	
	//Release the lock
	lock1 = 1;
}

//Clears sequence numbers for a flow specified by the given packet 
void clearSeqNums(packet * p1) {
	struct data_Queue * temp=roots;
	while(temp!=NULL) {
		if(memcmp((temp->p)->source,p1->source,4)==0 && memcmp((temp->p)->dest,p1->dest,4)==0 && p1->type==(temp->p)->type) {
			temp->p->seq_num = 0;
		}
		temp=temp->next;
	}
}

//Adds a new packet containing to the queue
void add_datapacketnode(packet * p1) {
	
	//Obtain the lock
	while(!lock1);
	lock1 = 0;
	
	int dup = 0;
	struct data_Queue * last, * temp;
	if(roots == NULL) {
		roots=(struct data_Queue*)malloc(sizeof(struct data_Queue));
		roots->p = p1;
		roots->next = NULL;
		roots->marked = 0;
	} else {
		temp = roots;
		
		//Check to make sure the packet is not a duplicate
		while(temp != NULL) {
			if(memcmp((temp->p)->source,p1->source,4) == 0 && memcmp((temp->p)->dest,p1->dest,4) == 0 && (temp->p)->seq_num == p1->seq_num && p1->type == (temp->p)->type ) {
				dup = 1;
				break;
			}
			last=temp;
			temp=temp->next;
		}
		
		//Add the packet if it is not a duplicate
		if(dup != 1) {
			last->next = (struct data_Queue*)malloc(sizeof(struct data_Queue));
			last = last->next;
			last->p = p1;
			last->next = NULL;
			last->marked = 0;
		}
	}
	
	//Release the lock
	lock1 = 1;
}

//Returns the largest sequence number packet received for a particular flow in the queue
int chk_seq(packet * p){

	//Obtain the lock
	while(!lock1);
	lock1 = 0;
	
	int high = 0;
	struct data_Queue * temp = roots;
	
	//Check all packets for the higest sequence number
	while(temp != NULL){
		if((memcmp(p->source,temp->p->source,4) == 0) && (memcmp(p->dest,temp->p->dest,4) == 0)){
			if(temp->p->seq_num > high) 			
				high=temp->p->seq_num;	
		}
		temp=temp->next;
	}
	
	//Release the lcok
	lock1=1;
	
	return high;
}

//Deletes all packets up to but not including the packet stop
void deletenode(struct data_Queue * stop) {

	//Obtain lock
	while(!lock1);
	lock1 = 0;
	
	data_Queue * temp, * prev, * temp2;
	temp = roots;
	prev = NULL;
	
	//Loop through queue until the packet stop is encountered
	while(temp != NULL) {
		if(temp == stop)
			break;
		if(temp->marked == 1) {
			if (prev == NULL) {
				roots = temp->next;
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

	//Release the lock
	lock1 = 1;
}

//Gets the oldest packet in the queue for the correspondng flow from srcip
packet * getOldestPacket(char * srcip) {

	//Obtain the lock
	while(!lock1);
	lock1 = 0;
	
	data_Queue * temp;
	temp = roots;
	int flag = 0, length1;
	
	//Loop through to find the packet, mark it for deletion, and delete all older packets
	while(temp != NULL) { 
		if((srcip == NULL || memcmp((temp->p)->source,srcip,4) == 0) && temp->marked == 0) {
			//Mark for deletion
			temp->marked = 1;
			
			//Release the lock
			lock1 = 1;
			
			//Delete packets older than this one
			deletenode(temp);
			
			return temp->p;
		}
		temp=temp->next;
	}
	
	//Release the lock
	lock1 = 1;
	
	return NULL;
}
