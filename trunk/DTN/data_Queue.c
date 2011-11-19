#include"dataQueue.h"



struct data_Queue *root = NULL;
int lock1=1;
void add_datapacketnode(packet *p1) {
	while(!lock1);
	lock1 = 0;
	printf("ITS from ");
	printIP(p1->source);
	printf("\n");
	printf("ITS TO ");
	printIP(p1->dest);
	printf("\n");	
	printf("Size : %d\n", p1->length - sizeof(packet) + MAX_FRAME_SIZE);

	int dup=0;
	struct data_Queue *node,*head3,*last;
	if(root==NULL) {
		node=(struct data_Queue*)malloc(sizeof(struct data_Queue));
		//printf("allocate %d\n", node);
		node->p=p1;
		node->next=NULL;
		node->marked = 0;
		root=node;
		printf("FGSDFDSFSD\n");
		//node->time_in = time(NULL);
		//printf("The packet goes in the list at %d with data %s\n",node->time_in,node->p->data);
	} else {
	printf("aaaaaaFGSDFDSFSD\n");
		head3=root;
		while(head3!=NULL) {
			if(memcmp((head3->p)->source,p1->source,4)==0 && memcmp((head3->p)->dest,p1->dest,4)==0 && (head3->p)->seq_num==p1->seq_num && p1->type==(head3->p)->type ) {
				dup=1;
				break;
			}
		last=head3;
		head3=head3->next;
		}
		if(dup!=1) {
			node=(struct data_Queue*)malloc(sizeof(struct data_Queue));
			node->p=p1;
			node->next=NULL;
			node->marked = 0;
			last->next=node;
			last=node;
			//node->time_in = time(NULL);
			//printf("The packet goes in the list at %d with data %s\n",node->time_in,node->p->data);
		}
	}
	
	//printf("");
	lock1 = 1;
}


int chk_seq(packet *p){
	printf("SEQ2: %d\n", p->seq_num);
	printf("SRC: ");
	printIP(p->source);
	printf("\n");
	printf("DST: ");
	printIP(p->dest);
	printf("\n");
	
	while(!lock1);
	lock1 = 0;
	
	
	int high=0;
	struct data_Queue *temp=root;
	while(temp!=NULL){
		if((memcmp(p->source,temp->p->source,4)==0)&&(memcmp(p->dest,temp->p->dest,4)==0)){
			printf("SRC: ");
	printIP(temp->p->source);
	printf("\n");
	printf("DST: ");
	printIP(temp->p->dest);
	printf("\n");
			if(temp->p->seq_num > high)
				high=temp->p->seq_num;	
		}
		temp=temp->next;
	}
	printf("THE HIGH WAS %d\n", high);
	lock1=1;
	return high;
}

void deletenode(struct data_Queue *stop) {
	while(!lock1);
	lock1 = 0;
	data_Queue *node,*head2,*temp,*prev;
	packet *p2;
	int result;
	if(root==NULL) 
	{
		printf("No packets in master queue right now. Aborted delete node operation");
                exit(1);
	} 
	else {
		head2=root;
		prev = NULL;
		
		while(head2!=NULL) {
			if(head2 == stop)
				break;
			p2=head2->p;
			if(head2->marked == 1) {
				printf("deleting packet from master queue!!\n");
				if (prev == NULL) {
					root = head2->next;
				} else {
					prev->next = head2->next;
				}
				temp=head2;
				head2=head2->next;
				//free(temp->p);
				//printf("free %d\n", temp);
				//free(temp);
				
			} else {
				prev = head2;
				head2=head2->next;
			}	
		}
	}
	lock1 = 1;
}

packet * getOldestPacket(char * srcip) {
while(!lock1);
lock1=0;
data_Queue *headnew,*headnew1,*nodenew;
packet *pnew;
headnew=root;
int flag=0,length1;

		if(headnew==NULL) 
		{
		//printf("There is nothing in the master queue\n");
		//exit(1);
		lock1 = 1;
		
		return NULL;
		} 
else
{

	while(headnew!=NULL)
	{ 
		if(memcmp((headnew->p)->source,srcip,4)==0 && headnew->marked == 0) 
		{
			nodenew=headnew;
			nodenew->marked=1;
			lock1=1;
			//deletenode(nodenew);
			
			return nodenew->p;
			flag=1;
		}
		headnew=headnew->next;
	}

	if(flag==1)
	nodenew->marked=1;
	//if(flag==0)
	//printf("There is no packet from the srcip inputted in the master queue\n");
	lock1=1;
	return NULL;
	



}
}