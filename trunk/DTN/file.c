#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"packet.h"
#include"queuestruc.h"
#include"sequence.h"
#include"UDP_server.h"
#include"bundle.h"

int main()
{		
char buffer[1024],*buffer1;
char *file,*destip,*srcip;
FILE *fp;
int bufferlen,len1,buffer_length,a,b,c,d;
	char DSTIP[4],SRCIP[4];

printf("DATA SEND OPN: Enter the file name and IP address to transfer the file to: \n");
scanf("%s",file, "%d.%d.%d.%d", &a, &b, &c, &d);
		DSTIP[0] = a;
		DSTIP[0] = b;
		DSTIP[0] = c;
		DSTIP[0] = d;;

//fp = fopen("data.txt","r");
fp=fopen(file,"r");

if (fp ==NULL) 
{
printf("File Not found \n");
exit(1);
}
//printf("reached");
		fseek(fp, 0, SEEK_END); // seek to end of file
	        buffer_length = ftell(fp); // get current file pointer
        	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

        	buffer1 = (char *) malloc(buffer_length);
		fread(buffer, sizeof(char), buffer_length, fp);
		//printf("data read from file:%s",buffer);
		DTN_datasend(DSTIP,buffer1,buffer_length);

printf("DATA SEND OPN: Enter IP address to receive the data packets from & the buffer length of data to be read from that IP: \n");
scanf("%d.%d.%d.%d", &a, &b, &c, &d,"%d",bufferlen);
		SRCIP[0] = a;
		SRCIP[0] = b;
		SRCIP[0] = c;
		SRCIP[0] = d;;

printf("DATA READ FROM SRCIP:\n");
		while(bufferlen>0)
		{		
	        DTN_datareceive(SRCIP,buffer,bufferlen);
		printf("%s",buffer);
		len1=strlen(buffer);
		bufferlen=bufferlen-len1;
		}
}
