#include<stdio.h>
#include<stdlib.h>
#include"packet.h"
#include"queuestruc.h"

void pushdata__queue(packet p1)
{
queue *node;
//packet p1;
//int len=strlen(recd_data);
//p1=(packet*)recd_data;
if(root==NULL)
{
node=(struct queue*)malloc(sizeof(queue));
node->p=p1;
node->next=NULL;
//head=root;
root=node;
head=root;
}

else
//add 1st data
{
//head=root;
//while(head->next!=NULL)
//head=head->next;
if(head->next==NULL)
{
node=(struct queue*)malloc(sizeof(queue));
node->p=p1;
node->next=NULL;
head->next=node;
head=node;
}

}

}


