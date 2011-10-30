#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>

int MainSocket;
int broadcast = 1;
struct sockaddr_in server_addr , client_addr;

int initialize() {
		MainSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(MainSocket == -1) {
			perror("Socket");
			exit(1);
		}
		

		if (setsockopt(MainSocket, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
       			 perror("setsockopt (SO_BROADCAST)");
       			 exit(1);
    		}

		server_addr.sin_family = AF_INET;
        	server_addr.sin_port = htons(8000);
        	server_addr.sin_addr.s_addr = INADDR_ANY;
        	bzero(&(server_addr.sin_zero),8);
		return MainSocket;
}

void *waitForPacket() {
		int listen_sock;
		int bytes_read;		
		char recv_data[1024];
		int addr_len = sizeof(struct sockaddr);		
		listen_sock = MainSocket;
		
        	if (bind(listen_sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
        	{
            		perror("Bind");
            		exit(1);
        	}
		printf("\nUDPServer Waiting for client on port 8000");
	        fflush(stdout);
		while(1) {
			bytes_read = recvfrom(listen_sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
	  
		  	recv_data[bytes_read] = '\0';

	          	printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	          	printf("%s", recv_data);
		  	fflush(stdout);
		}
}

void *sendPackets() {
		int send_sock;
		char ip[]={10,10,10,255};
		char data[1024];
		send_sock = MainSocket;
		server_addr.sin_addr.s_addr = inet_addr("10.10.10.255");
		server_addr.sin_port = htons(8000);
		printf("Sending to %s on port %d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
		while(1) {
		
		gets(data);
		if ((strcmp(data , "q") == 0) || strcmp(data , "Q") == 0)
       			break;

    		else
       			sendto(send_sock, data, strlen(data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

     		}
}

int main(int argc, char *argv[])
{
	/*if(atoi(argv[1]) == 0)
	{
		printf("Server mode\n");
		waitForPacket();
	}
	else
	{
		printf("Client mode\n");
		sendPackets();
	}*/
	initialize();
	pthread_t sender,receiver;
	pthread_create(&sender,NULL,sendPackets,NULL);
	pthread_create(&receiver,NULL,waitForPacket,NULL);
	while(1);
	return 0;
}
