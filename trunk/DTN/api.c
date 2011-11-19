#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataQueue.h"
#include "packet.h"
#include "queuestruc.h"
#include "UDP_server.h"
#include "sequence.h"
#include "bundle.h"

int locknew=0;

void DTN_datasend(char *destip,char *fulldata, int len)
{
char *buffer;
char* bufferdata,*file;
packet *pnew;
FILE *fp;
int num;
int i=0,j=0,numb,start,end,buffer_length;
/*		
printf("Enter the file name and IP address to tranfer the file to: \n");
//scanf("%s %s", file, destip);

fp = fopen("data.txt","r");

if (fp ==NULL) {
printf("File Not found \n");
exit(1);
}
//printf("reached");
		fseek(fp, 0, SEEK_END); // seek to end of file
	        buffer_length = ftell(fp); // get current file pointer
        	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

        	buffer = (char *) malloc(buffer_length);

		fread(buffer, sizeof(char), buffer_length, fp);
*/
		num=(len/MAX_FRAME_SIZE)+1;
		printf("length of to be pushed data:%d   num:%d",len,num);
		while(i<num)
		{
		j=0;
		bufferdata=(char*)malloc(MAX_FRAME_SIZE);
		start=i*MAX_FRAME_SIZE;
		end=(i+1)*MAX_FRAME_SIZE;
		while(start<=end)
		{
		bufferdata[j]=fulldata[start];
		start++;	
		j++;
		if(fulldata[start]==NULL)break;
		}		
		newPacket(destip, bufferdata,strlen(bufferdata));
		i++;
//		printf("\nbuffer of 10 bytes is:%s\n",bufferdata);
		free(bufferdata);
		}
}


int minm(int a, int b)
{
if(a>b) return a;
return b;
} 

char* DTN_datareceive(char *srcip,char *buffer,int bufferlen)
{
data_Queue *headnew,*headnew1,*nodenew;
packet *pnew;
headnew=root;
int flag=0,length1;

		if(headnew==NULL) 
		{
		printf("There is nothing in the master queue\n");
		exit(1);
		} 
else
{

	while(headnew!=NULL)
	{
	if(memcmp((headnew->p)->source,srcip,4)==0) 
	{nodenew=headnew;
	flag=1;}
	headnew=headnew->next;
	}

	if(flag==1)
	nodenew->marked=1;
	if(flag==0)
	printf("There is no packet from the srcip inputted in the master queue\n");

while(headnew1!=NULL)
{
	if(memcmp((headnew1->p)->source,srcip,4)==0 && headnew1->marked==0) 
	{
	length1=minm((headnew1->p)->length,bufferlen);
	strncpy(buffer,headnew1->p->data,length1);
	deletenode(headnew1->p);
	break;
	}

	if(memcmp((headnew1->p)->source,srcip,4)==0 && headnew1->marked==1) 
	{
	length1=minm((headnew1->p)->length,bufferlen);
	strncpy(buffer,headnew1->p->data,length1);
	break;
	}

headnew1=headnew1->next;
}

}
return buffer;
}


void deletenode(packet *p1) {
	while(!locknew);
	locknew = 0;
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
			p2=head2->p;
			if(head2->marked == 0) {
				printf("deleting packet from master queue!!\n");
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
	}
	locknew = 1;
}


