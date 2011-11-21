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
#include<stdlib.h>
#include<sys/ioctl.h>
#include"packet.h"
#include"queuestruc.h"
#include"sequence.h"
#include"DTN.h"
#include"bundle.h"
	
#define INTERVAL 1000		/* number of milliseconds to go off */

int MainSocket;
int broadcast = 1;
struct sockaddr_in server_addr , client_addr;
char *my_ip[5],*temp;
int gateway = 0;
char net[4];
char mask[4];


void setGateway(char * ip, char * netMask) {
	gateway = 1;
	memcpy(mask, netMask, 4);
	printf("I am a gateway on the network ");
	net[0] = ip[0] & mask[0];
	net[1] = ip[1] & mask[1];
	net[2] = ip[2] & mask[2];
	net[3] = ip[3] & mask[3];
	printIP(net);
	printf("\n");
}

void * oldMain() {
	struct itimerval it_val;	/* for setting itimer */
    	pthread_t sender,receiver;
	pthread_create(&receiver,NULL,waitForPacket,NULL);
	if (signal(SIGALRM, (void (*)(int)) send_beacon) == SIG_ERR) 
	{
    		perror("Unable to catch SIGALRM");
    		exit(1);
  	}
  	it_val.it_value.tv_sec =     INTERVAL/1000;
  	it_val.it_value.tv_usec =    (INTERVAL*1000) % 1000000;	
  	it_val.it_interval = it_val.it_value;
  	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) 
	{
  		  perror("error calling setitimer()");
  		  exit(1);
  	}
 	while (1) 
   		pause();
	return 0;
}

void get_my_ip(char interface[]) {
	char hostbuf[256];
        /*struct hostent *hostentry;
        int ret;
        ret = gethostname(hostbuf,sizeof(hostbuf));
        if(-1 == ret)
	{
                perror("gethostname");
                exit(1);
        }

        hostentry = gethostbyname(hostbuf);

        if(NULL == hostentry)
	{
                perror("gethostbyname");
                exit(1);
        }
	*/

	struct ifreq ifr;
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;	
	strcpy(ifr.ifr_name, interface);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);

        temp = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);	
        if(NULL == my_ip)
	{
                perror("inet_ntoa");
                exit(1);
        }
	int i =0;
	my_ip[0] = (char*)strtok(temp,".");
		
	while (my_ip[i])
	{
		 i++;
		 my_ip[i] = strtok (NULL, ".");
	}
		
	ipp[0]=atoi(my_ip[0]);
	ipp[1]=atoi(my_ip[1]);
	ipp[2]=atoi(my_ip[2]);
	ipp[3]=atoi(my_ip[3]);
}



int initialize(char interface[]) {
		struct in_addr a;
		char host[100];
		gethostname(host,sizeof(host));	
		struct hostent *name=gethostbyname(host);
		get_my_ip(interface);
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
			
		pthread_t run;
		pthread_create(&run,NULL,oldMain,NULL);
	
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
		
	        fflush(stdout);
		while(1) {
			
			struct Apacket *packet1;
			packet1 = (packet *)malloc(sizeof(packet));
		
			bytes_read = recvfrom(listen_sock,packet1,1024,0,(struct sockaddr *)&client_addr, &addr_len);

			if(memcmp(packet1->source, ipp, 4))
			{	
			if(packet1->type == TYPE_BEACON) {
				send_all(inet_ntoa(client_addr.sin_addr));
				memset(packet1,NULL,sizeof(packet));
				free(packet1);
			}
			else if(packet1->type == TYPE_DATA) {
				//printf("Received DATA with ttl %d\n",packet1->ttl);
				data_handler(packet1);
			}
			else if(packet1->type == TYPE_ACK) {
				//printf("Received ACK with ttl %d\n",packet1->ttl);
				ack_handler(packet1);
			}
			}
		  	fflush(stdout);
		}
}

void sendPackets(struct Apacket *packet, char *ip) {
	int send_sock;
	send_sock = MainSocket;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(8000);
	bzero(&(server_addr.sin_zero),8);
	sendto(send_sock, (char *)packet, packet->length, 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
}



void send_beacon() {
	print_all();
	struct Apacket *packet = (struct Apacket *)malloc(sizeof(struct Apacket));
	packet->type = 1;
	packet->seq_num = 0;
	sprintf(packet->data,"Beacon\n");
	packet->source[0]=ipp[0];
	packet->source[1]=ipp[1];
	packet->source[2]=ipp[2];
	packet->source[3]=ipp[3];
	packet->length = sizeof(struct Apacket)-MAX_FRAME_SIZE+strlen(packet->data);
	sendPackets(packet,"192.168.1.255");
	free(packet);
}

void data_handler(struct Apacket *packet) {
	/*	Got packet ---- Update seq num ---- If 1 -> store data in linked list, else return */
	struct Apacket *ack;
	int takePacket = 0;
		
	if(isOld(packet)==NOT_OLD_PACKET) {
		takePacket = 0;
		if (gateway) {
			char pNet[4];
			pNet[0] = packet->dest[0] & mask[0];
			pNet[1] = packet->dest[1] & mask[1];
			pNet[2] = packet->dest[2] & mask[2];
			pNet[3] = packet->dest[3] & mask[3];
			if (pNet[0] != net[0] || pNet[1] != net[1] || pNet[2] != net[2] || pNet[3] != net[3]) {
				takePacket = 1;
			}
		}
		if(!memcmp(packet->dest,ipp,4) || takePacket==1)//The received packet is destined for me.
		{	
			ack=deliverPacket(packet);	//Get the ACK from the bundle layer
			if(ack!=NULL){
				isOld(ack);
				add_packetnode(ack);
				delete_packetnode(ack);
			}
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
		if(memcmp(packet->dest,ipp,4)!=0)
		{
				//printf("ACK IS NOT FOR ME");
				add_packetnode(packet);
		}
		else 
		{
			//printf("ACK IS FOR ME... NOT ADDING TO LIST\n");
		}
                delete_packetnode(packet);
		print_all();
	}
	return ;
}



