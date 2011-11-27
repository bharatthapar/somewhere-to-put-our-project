#include"dataQueue.h"



struct data_Queue *roots = NULL;
int lock1=1;

void print_all1() {
	while(!lock1);
	lock1 = 0;
	data_Queue *head3;
	int i=0;
	head3=roots;
	int data = 0;
	int ack = 0;
	//printf("Printing packets in the queue:\n");
	while(head3!=NULL) {
		//printf("----Packet-----\n");
		//printf("type: %d\n",head3->p->type);
		//printf("source: ");
		//printIP(head3->p->source);
		//printf("\n");
		//printf("dest: ");
		//printIP(head3->p->dest);
		//printf("\n");
		//printf("sequence: %d",head3->p->seq_num);
//		printf(" %s\n",head3->p->data);
		//printf("---End Packet---\n");
		if (head3->p->type == TYPE_DATA)
			data++;
		else if (head3->p->type == TYPE_ACK)
			ack++;
		
		head3=head3->next;
		i++;
	}
	system("clear");
	//printf("My IP is ");
	//printIP(ipp);
	printf("\n");
	printf("Number of data packets present: %d\n", data);
	printf("Number of ACKS present %d\n", ack);
	//printf("Queue ends\n");
	lock1=1;
}


void clearSeqNums(packet * p1) {
	struct data_Queue * temp=roots;
	while(temp!=NULL) {
		if(memcmp((temp->p)->source,p1->source,4)==0 && memcmp((temp->p)->dest,p1->dest,4)==0 && p1->type==(temp->p)->type ) {
			temp->p->seq_num = 0;
			printf("SET TO ZERO!!!\n");
		}
		temp=temp->next;
	}
}



void add_datapacketnode(packet *p1) {
	//printf("ADDING!!\n");
	while(!lock1);
	lock1 = 0;
	int dup=0;
	struct data_Queue *last, *temp;
	if(roots==NULL) {
		roots=(struct data_Queue*)malloc(sizeof(struct data_Queue));
		roots->p=p1;
		roots->next=NULL;
		roots->marked = 0;
	} else {
		temp=roots;
		while(temp!=NULL) {
			if(memcmp((temp->p)->source,p1->source,4)==0 && memcmp((temp->p)->dest,p1->dest,4)==0 && (temp->p)->seq_num==p1->seq_num && p1->type==(temp->p)->type ) {
				dup=1;
				break;
			}
		last=temp;
		temp=temp->next;
		}
		if(dup!=1) {
			//printf("Not a duplicate packet\nADDING it to data_queue\n");
			last->next=(struct data_Queue*)malloc(sizeof(struct data_Queue));
			last=last->next;
			last->p=p1;
			last->next=NULL;
			last->marked = 0;
		}
	}
	lock1 = 1;
}

int chk_seq(packet *p){
	while(!lock1);
	lock1 = 0;
	int high=0;
	struct data_Queue *temp=roots;
	while(temp!=NULL){
		if((memcmp(p->source,temp->p->source,4)==0)&&(memcmp(p->dest,temp->p->dest,4)==0)){
			if(temp->p->seq_num > high) 			
				high=temp->p->seq_num;	
		}
		temp=temp->next;
	}
	lock1=1;
	printf("THE HIGH IS %d\n", high);
	return high;
}

void deletenode(struct data_Queue *stop) {
	while(!lock1);
	lock1 = 0;
	data_Queue *temp,*prev,*temp2;
	temp=roots;
	prev = NULL;
		
	while(temp!=NULL) {
		if(temp == stop)
			break;
		if(temp->marked == 1) {
			if (prev == NULL) {
				roots = temp->next;
			} else {
				prev->next = temp->next;
			}
			temp2=temp;
			temp=temp->next;
			free(temp2->p);
			//printf("free %d\n", temp);
			free(temp2);
			
		} else {
			prev = temp;
			temp=temp->next;
		}	
	}
	lock1 = 1;
}

packet * getOldestPacket(char * srcip) {
	while(!lock1);
	lock1=0;
	data_Queue *temp;
	temp=roots;
	int flag=0,length1;
	
	while(temp!=NULL) { 
		if((srcip == NULL || memcmp((temp->p)->source,srcip,4)==0) && temp->marked == 0) {
			temp->marked=1;
			lock1=1;
			deletenode(temp);
			return temp->p;
		}
		temp=temp->next;
	}
	lock1=1;
	return NULL;
}
