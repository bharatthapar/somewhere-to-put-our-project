#include<dataQueue.h>


dataQueue *root;
int lock = 1;
void add_datapacketnode(packet *p1) {
	
	while(!lock);
	lock = 0;
	int dup=0;
	queue *node,*head3,*last;
	if(root==NULL) {
		node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
		//printf("allocate %d\n", node);
		node->p=p1;
		node->next=NULL;
		node->marked = 0;
		root=node;
		node->time_in = time(NULL);
		//printf("The packet goes in the list at %d with data %s\n",node->time_in,node->p->data);
	} else {
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
			node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
			node->p=p1;
			node->next=NULL;
			node->marked = 0;
			last->next=node;
			last=node;
			node->time_in = time(NULL);
			//printf("The packet goes in the list at %d with data %s\n",node->time_in,node->p->data);
		}
	}
	
	//printf("");
	lock = 1;
}


int chk_seq(packet *p){
	int high=-1;
	struct data_Queue *temp=root;
	while(temp!=NULL){
		if((memcmp(p->source,temp->p->source,4)==0)&&(memcmp(p->dest,temp->dest,4)==0)){
			if(temp->p->seq_num > high)
				high=temp->p->seq_num;	
		}
		temp=temp->next;
	}
	return high;
}


