#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<net/if.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<netdb.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>

#include<sys/ioctl.h>

#include"packet.h"
#include"queuestruc.h"
#include"sequence.h"
#include"UDP_server.h"
#include"bundle.h"
#define INTERVAL 5000		/* number of milliseconds to go off */

int MainSocket;
int broadcast = 1;
struct sockaddr_in server_addr , client_addr;
char *my_ip;

void get_my_ip() {
	 int fd;
	 struct ifreq ifr;

	 fd = socket(AF_INET, SOCK_DGRAM, 0);

	 /* I want to get an IPv4 IP address */
	 ifr.ifr_addr.sa_family = AF_INET;

	 /* I want IP address attached to "eth0" */
	 strncpy(ifr.ifr_name, "eth1", IFNAMSIZ-1);
	
	 ioctl(fd, SIOCGIFADDR, &ifr);
	
	 close(fd);
	 my_ip=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
	
}



int initialize() {
		struct in_addr a;
		char host[100];
		gethostname(host,sizeof(host));	
		
		printf("Host is %s\n\n",host);	
		struct hostent *name=gethostbyname(host);
		get_my_ip();
		printf("IP is %s\n",my_ip);
		while (*name->h_addr_list)
        	{
            		bcopy(*name->h_addr_list++, (char *) &a, sizeof(a));
		        printf("address: %s\n", inet_ntoa(a));
        	}
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
		struct Apacket *packet1;
		packet1 = malloc(sizeof(packet));
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
			printf("Im here as well\n");		
			bytes_read = recvfrom(listen_sock,packet1,1024,0,(struct sockaddr *)&client_addr, &addr_len);
	  		printf("here too\n");
		  	//recv_data[bytes_read] = '\0';

	          	printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	          	printf("\n%s\n", packet1->data);

			if(packet1->type == TYPE_BEACON) {
				send_all(packet1->source);
			}
			else if(packet1->type == TYPE_DATA) {
				data_handler(packet1);
			}
			else if(packet1->type == TYPE_ACK) {
				ack_handler(packet1);
			}
		  	fflush(stdout);
		}
}

void sendPackets(struct Apacket *packet, char *ip) {
		int send_sock;
		send_sock = MainSocket;
		server_addr.sin_addr.s_addr = inet_addr(ip);
		server_addr.sin_port = htons(8000);
		printf("Sending to %s on port %d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
		sendto(send_sock, (char *)packet, packet->length, 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		printf("bhej diya\n");
}

int main(int argc, char *argv[])
{
	struct itimerval it_val;	/* for setting itimer */
    	initialize();
	pthread_t sender,receiver;
	//pthread_create(&sender,NULL,sendPackets,NULL);
	pthread_create(&receiver,NULL,waitForPacket,NULL);
	if (signal(SIGALRM, (void (*)(int)) send_beacon) == SIG_ERR) {
    			perror("Unable to catch SIGALRM");
    			exit(1);
  	}
  	it_val.it_value.tv_sec =     INTERVAL/1000;
  	it_val.it_value.tv_usec =    (INTERVAL*1000) % 1000000;	
  	it_val.it_interval = it_val.it_value;
  	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
  		  perror("error calling setitimer()");
  		  exit(1);
  	}

 	while (1) 
   		pause();
	return 0;
}

void send_beacon() {
	printf("Im in\n");
	struct Apacket packet;
	packet.type = 1;
	packet.seq_num = 0;
	packet .ttl = 2;
	sprintf(packet.data,"Beacon\n");
	printf("%s\n",packet.data);
	packet.length = sizeof(packet);
	sendPackets(&packet,"255.255.255.255");
}

void data_handler(struct Apacket *packet) {
	/*	Got packet ---- Update seq num ---- If 1 -> store data in linked list, else return */
	struct Apacket *ack;
	if(isOld(packet)==NOT_OLD_PACKET) {
		if(!memcmp(packet->dest,my_ip,4))	//The received packet is destined for me.
		{		
			ack=deliverPacket(packet);	//Get the ACK from the bundle layer
			isOld(ack);
			add_packetnode(ack);
		}
		else 
		{
			add_packetnode(packet);
		}
	}
	return ;
}

void ack_handler(struct Apacket *packet) {
	/*Got ACK ---- Update seq num ---- If 1 -> store ACK in linked list....remove all old data and ACK packets, else return0*/

	if(isOld(packet)==NOT_OLD_PACKET) {
		add_packetnode(packet);
                delete_packetnode(packet);
	}
	return ;
}



