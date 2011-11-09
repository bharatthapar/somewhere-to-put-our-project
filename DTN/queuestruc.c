#include<stdio.h>
#include<stdlib.h>
#include"queuestruc.h"
#include"packet.h"
#include"UDP_server.h"
#include"sequence.h"

void sendPackets(struct Apacket *packet, char *ip){}

void add_packetnode(packet *p1)
{
queue *node;
if(root==NULL)
{
node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
node->p=*p1;
node->prev=NULL;
node->next=NULL;
root=node;
head=root;
}

else
{
if(head->next==NULL)
{
node=(struct dataqueue*)malloc(sizeof(struct dataqueue));
node->p=*p1;
node->prev=head;
node->next=NULL;
head->next=node;
head=node;
}
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
sendPackets(&(head2->p), serverip);
head2=head2->next;
}
//return 0;
}

void send_packetnode(packet *p1,char *serverip)
{
queue *node,*head2;
packet p2;

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
sendPackets(&(head2->p), serverip);
break;
}
}}

}

void delete_packetnode(packet *p1)
{
queue *node,*head2;
packet p2;

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
if(p2.type==TYPE_ACK && strcmp(p2.source,p1->source)==0 && strcmp(p2.dest,p1->dest)==0 && p2.seq_num<p1->seq_num)
{
(head2->prev)->next=head2->next;
(head2->next)->prev=head2->prev;
free(head2);
}
if(p2.type==TYPE_DATA && strcmp(p2.source,p1->source)==0 && strcmp(p2.dest,p1->dest)==0 && p2.seq_num<=p1->seq_num)
{
(head2->prev)->next=head2->next;
(head2->next)->prev=head2->prev;
free(head2);
}
}}
}



