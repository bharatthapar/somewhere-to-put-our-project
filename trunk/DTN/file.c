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
char *buffer;
char *file,*destip,*srcip;
FILE *fp;
int bufferlen,len1,buffer_length;

printf("DATA SEND OPN: Enter the file name and IP address to tranfer the file to: \n");
scanf("%s %s", file, destip);

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

        	buffer = (char *) malloc(buffer_length);
		fread(buffer, sizeof(char), buffer_length, fp);
		//printf("data read from file:%s",buffer);
		DTN_datasend(destip,buffer,buffer_length);

printf("DATA SEND OPN: Enter IP address to receive the data packets from & the buffer length of data to be read from that IP: \n");
scanf("%s %s", srcip,bufferlen);
printf("DATA READ FROM SRCIP:\n");
		while(bufferlen>0)
		{
		buffer=(char*)malloc(sizeof(bufferlen);
	        DTN_datareceive(srcip,buffer,bufferlen);
		printf("%s",buffer);
		len1=strlen(buffer);
		bufferlen=bufferlen-len1;
		delete buffer;
		}
}
