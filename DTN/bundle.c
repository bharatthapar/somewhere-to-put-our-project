#include "bundle.h"
#include "queuestruc.h"
#include "packet.h"
#include "UDP_server.h"
#include "sequence.h"
#include <stdio.h>
#include <string.h>
#include "tunnel.h"
#include "dataQueue.h"

int dtn0;



packet * deliverPacket(packet * p) {
	//printf("SEQ1: %d\n", p->seq_num);
	
	packet * ack = NULL;
	int seq = chk_seq(p);
	if(seq == 0 || seq == p->seq_num-1)
	{
	
		add_datapacketnode(p);
		ack = malloc(sizeof(packet));
		memcpy(ack->dest, p->source, 4);
		memcpy(ack->source, p->dest, 4);
		ack->type = TYPE_ACK;
		ack->ttl = 20;
		sprintf(ack->data, "ACK");
		ack->length = sizeof(packet) - MAX_FRAME_SIZE + 3;
		ack->seq_num = p->seq_num;
		isOld(ack);
		//free packets
		//free(p);
		
		return ack;
	}
	else
		return NULL;
}



void newPacket(char * dest, char * data, int len) {
	if (len == 10)
	printf("New packet (len: %d) (data %c%c%c%c%c%c%c%c%c%c)\n", len, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
	else
	printf("New packet (len: %d) (data %c%c)\n", len, data[0], data[1]);
	//return;
	packet * p = malloc(sizeof(packet));
	p->type = TYPE_DATA;
	memcpy(p->dest, dest, 4);
	memcpy(p->source, ipp, 4);
	memcpy(p->data, data, len);
	p->ttl = 20;
	p->length = sizeof(packet) - MAX_FRAME_SIZE + len;
	addSequenceNumber(p);
	add_packetnode(p);
}


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
		//printf("length of to be pushed data:%d   num:%d\n",len,num);
		while(i<num)
		{
		//j=0;
		//bufferdata=(char*)malloc(MAX_FRAME_SIZE);
		//start=i*MAX_FRAME_SIZE;
		//end=(i+1)*MAX_FRAME_SIZE;
		//while(start<=end)
		//{
		//bufferdata[j]=fulldata[start];
		//start++;	
		//j++;
		//if(fulldata[start]==NULL)break;
		//}		
		//newPacket(destip, bufferdata,strlen(bufferdata));
		int length = i==num-1?len-i*MAX_FRAME_SIZE:MAX_FRAME_SIZE;
		if (length > 0)
		newPacket(destip, &fulldata[MAX_FRAME_SIZE*i],length);
		i++;
//		printf("\nbuffer of 10 bytes is:%s\n",bufferdata);
		//free(bufferdata);
		}
}


int minm(int a, int b)
{
if(a>b) return a;
return b;
} 

int DTN_datareceive(char *srcip,char *buffer,int bufferlen)
{
	printf("Receive from ");
	printIP(srcip);
	printf("\n");
	
	int flag=0,length1;
	packet * p = NULL;
	
	while(p == NULL) 
		p = getOldestPacket(srcip);
	
	printf("IM HERE\n");
	
	//if (p==NULL)
	//return NULL;
	
	memcpy(buffer,p->data,minm(p->length - sizeof(packet) + MAX_FRAME_SIZE,bufferlen));

	//free(p);
	
return minm(p->length - sizeof(packet) + MAX_FRAME_SIZE,bufferlen);;
}




/*
int main(int argv, char * args[]) {
	packet p1, p2, p3, p4;
	
	p1.source[0] = 55;
	p1.source[1] = 56;
	p1.source[2] = 57;
	p1.source[3] = 58;
	p1.dest[0] = 5;
	p1.dest[1] = 6;
	p1.dest[2] = 7;
	p1.dest[3] = 8;
	p1.type = TYPE_DATA;
	memcpy(&p2,&p1,sizeof(packet));
	memcpy(&p3,&p1,sizeof(packet));
	memcpy(&p4,&p1,sizeof(packet));
	p1.seq_num = 55;
	p2.seq_num = 56;
	p3.seq_num = 57;
	p4.source[0] = 8;
	p4.seq_num = 999;
	p1.data[0] = '1';
	p2.data[0] = '2';
	p3.data[0] = '3';
	p4.data[0] = '4';
	
	//memcpy(p1.data, "11HELLO I HAVE 1DATA!!!!!!!!",28);
	//memcpy(p2.data, "22HELLO I HAVE 2DATA!!!!!!!!",28);
	//memcpy(p3.data, "33HELLO I HAVE 3DATA!!!!!!!!",28);
	//memcpy(p4.data, "44HELLO I HAVE 4DATA!!!!!!!!",28);
	p1.length = sizeof(packet) - MAX_FRAME_SIZE + strlen(p1.data);
	p2.length = sizeof(packet) - MAX_FRAME_SIZE + strlen(p2.data);
	p3.length = sizeof(packet) - MAX_FRAME_SIZE + strlen(p3.data);
	p4.length = sizeof(packet) - MAX_FRAME_SIZE + strlen(p4.data);
	//p1.seq_num = 55;
	//p2.seq_num = 56;
	//p3.seq_num = 57;
	
//printf("SEQ0: %d\n", (&p1)->seq_num);
printf("SEQ00: %d\n", p1.seq_num);
	
	packet * a1 = deliverPacket(&p1);
	printf("ACK: %d\n", a1);	
	
	a1 = deliverPacket(&p3);
	printf("ACK: %d\n", a1);
	
	
	a1 = deliverPacket(&p2);
	printf("ACK: %d\n", a1);
	
	a1 = deliverPacket(&p3);
	printf("ACK: %d\n", a1);
	
	//a1 = deliverPacket(&p4);
	//printf("ACK: %d\n", a1);

	
	
	char data[5200] ="This is 52 characters but it is not long enougth ha.";
	//DTN_datasend(p4.source, data, 42);
	
	DTN_datareceive(p1.source, data, 18);
	data[18] = '\0';
	printf("The data is '%s'\n", data);
	DTN_datareceive(p1.source, data, 18);
	data[18] = '\0';
	printf("The data is '%s'\n", data);
	DTN_datareceive(p1.source, data, 18);
	data[18] = '\0';
	printf("The data is '%s'\n", data);
	DTN_datareceive(p1.source, data, 18);
	data[18] = '\0';
	printf("The data is '%s'\n", data);	
	
	//	a1 = deliverPacket(&p3);
	//printf("ACK: %d\n", a1);
	exit(0);
	
	
	if (! strcmp(args[1],"1")) {
		printf("GATEWAY!!!\n");
		char ip[4];
		char mask[4] = {255,255,255,255};
		char * my_ip[5];
		int i =0, j=0;
		sscanf(args[2], "%d.%d.%d.%d/%d", &ip[0], &ip[1], &ip[2], &ip[3], &i);
		//sscanf(args[3], "%d.%d.%d.%d", &mask[0], &mask[1], &mask[2], &mask[3]);
		
		while (i > 8) {
			mask[j] = 255;
			i = i - 8;
			j++;
		}
		mask[j] = mask[j] << (8-i);
		for (j=j+1; j < 4; j++)
			mask[j] = 0;
		printIP(ip);
		printf("\n");
		printIP(mask);
		printf("\n");		
		setGateway(ip, mask);
		
		//exit(0);
	}

	char *interface = malloc(sizeof(args[1])+1);
	//strcpy(interface,args[1]);
	//printf("Interface is %s\n",interface);
	
	dtn0 = tun_init("dtn0");
	char ipbuf[200] = "ifconfig dtn0 ";
	
	strcat(ipbuf, args[2]);
	strcat(ipbuf, " up");
	
	system(ipbuf);

	//initialize(interface);
	initialize("dtn0");
	printf("My IP is ");
	printIP(ipp);
	printf("\n");
  char buf[1000] = "Hello I am a string.";
  char * dest = malloc(4);
  dest[0] = 192;
  dest[1] = 168;
  dest[2] = 7;
  dest[3] = 20;
  
 char * src = malloc(4);
  src[0] = 192;
  src[1] = 168;
  src[2] = 7;
  src[3] = 25;
  if (! strcmp(args[1],"2")){
	while(1){
		getchar();
		DTN_datasend(src, buf, strlen(buf));
		//packet * p = getPacket(dtn0);
		//if (p != NULL) {
		//	add_packetnode(p);
		//}
	}
	} else {
	while(1){
		DTN_datareceive(dest, buf, 100);
		buf[15] = '\0';
		printf("DATA: '%s'\n");
		}
	}
}
*/
