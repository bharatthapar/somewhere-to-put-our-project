#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bundle.h"
#include"dataQueue.h"

void * readData() {
	char SRCIP[4],buffer[1500];
	int i1=0,a,b,c,d,bufferlen,len1;
	printf("DATA RECV OPN: Enter IP address to receive the data packets from & the buffer length of data to be read from that IP: \n");
	scanf("%d.%d.%d.%d", &a, &b, &c, &d);
		SRCIP[0] = a;
		SRCIP[1] = b;
		SRCIP[2] = c;
		SRCIP[3] = d;
//print_all1();
	printf("DATA READ FROM SRCIP:\n");
		while(1)
		{

for(i1=0;i1<1500;i1++)
{
buffer[i1]='\0';
}
	        len1=DTN_datareceive(SRCIP,buffer,1500);

//		len1=strlen(buffer);
//		bufferlen=bufferlen-len1;*/
		printf("%s",buffer);
		}
	}

void * sendData() {
        char DSTIP[4],file[100],*buffer1;
	int a,b,c,d,buffer_length;
	FILE *fp;
	printf("DATA SEND OPN: Enter the file name and IP address to transfer the file to: \n");
	scanf("%s %d.%d.%d.%d",file,&a, &b, &c, &d);
		DSTIP[0] = a;
		DSTIP[1] = b;
		DSTIP[2] = c;
		DSTIP[3] = d;;

//fp = fopen("data.txt","r");
fp=fopen(file,"r");

if (fp ==NULL) 
{
printf("File Not found \n");
exit(1);
}
		fseek(fp, 0, SEEK_END); // seek to end of file
	        buffer_length = ftell(fp); // get current file pointer
        	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

        	buffer1 = (char *) malloc(buffer_length);
		fread(buffer1, sizeof(char), buffer_length, fp);
		DTN_datasend(DSTIP,buffer1,buffer_length);
}


int main(int argv, char * args[])
{		
int flag;
//char buffer[1024],*buffer1;
//char *file,*destip,*srcip;
//FILE *fp;
//int bufferlen,len1,buffer_length,a,b,c,d;
//	char DSTIP[4],SRCIP[4];
flag=atoi(args[1]);
createBundleLayer(args[2]);
	pthread_t sender,receiver;

	if(flag==1)
	pthread_create(&sender,NULL,sendData,NULL);
	if(flag==2)
	pthread_create(&receiver,NULL,readData,NULL);
	
while(1);
return 0;
}
