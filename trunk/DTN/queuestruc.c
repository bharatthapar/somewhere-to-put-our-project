#include<stdio.h>
#include<stdlib.h>

#include"queuestruc.h"
#include"packet.h"
#include"UDP_server.h"
#include"sequence.h"

void add_packetnode(packet *p1)
{
queue *node,*head3,*last;
if(root==NULL)
{
node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
node->p=p1;
node->prev=NULL;
node->next=NULL;
root=node;
head=root;
}

else
{
head3=head;
while(head3!=NULL)
{
if(strcmp((head3->p)->source,p1->source)==0 && strcmp((head3->p)->dest,p1->dest)==0 && (head3->p).seq_num==p1->seq_num)
{
printf("Duplicate Packet.. won't add it to queue");
break;
}
last=head3;
head3=head3->next;
}

node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
node->p=p1;
node->prev=last;
node->next=NULL;
last->next=node;
last=node;
}
}



void send_all(char *serverip)
{
queue *head2;

if(root==NULL)
{
printf("No packets in queue right now. Aborted send packets operation");
//return -1;
}

head2=root;
while(head2!=NULL)
{
sendPackets((head2->p), serverip);
head2=head2->next;
}
//return 0;
}

/*
void send_packetnode(packet *p1,char *serverip)
{
queue *node,*head2;
packet *p2;

if(root==NULL)
{
printf("No packets in queue right now. Aborted send packet node operation");
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

void delete_packetnode(packet *p1)
{
queue *node,*head2,*temp;
packet *p2;
int result;
if(root==NULL)
{
printf("No packets in queue right now. Aborted delete node operation");
}

else
{
head2=root;
while(head2!=NULL)
{
p2=head2->p;
result=keepPacket(p1,p2);
if(result==0)
{
(head2->prev)->next=head2->next;
(head2->next)->prev=head2->prev;
temp=head2;
free(temp);
}
head2=head2->next;
}
}

}



