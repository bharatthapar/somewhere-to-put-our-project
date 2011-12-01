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
#include "config.c"

int MainSocket;
struct sockaddr_in server_addr , client_addr;
int gateway = 0;
char net[4];
char mask[4];
config * configuration;

/* Function to set the gateway IP and netmask from config file */
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

/*Function for starting the receiver thread and setting the timer ALARM for Beacon sender */
void * oldMain() {
	struct itimerval it_val;	/* for setting itimer */
   	pthread_t sender,receiver;
	pthread_create(&receiver,NULL,waitForPacket,NULL);
	if (signal(SIGALRM, (void (*)(int)) send_beacon) == SIG_ERR) {
    		perror("Unable to catch SIGALRM");
    		exit(1);
  	}
  	it_val.it_value.tv_sec = configuration->beaconInterval/1000;
  	it_val.it_value.tv_usec = (configuration->beaconInterval*1000) % 1000000;	
  	it_val.it_interval = it_val.it_value;
  	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
  		  perror("error calling setitimer()");
  		  exit(1);
  	}
 	while (1) 
   		pause();
	return 0;
}

/* Function to initialize the self parameters for the connection. These parameters are passed from a config file. */
int initialize(config * c) {
	int broadcast = 1;
	configuration =  c;
	struct in_addr a;
	char host[100];
	gethostname(host,sizeof(host));	
	struct hostent *name=gethostbyname(host);
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
   	server_addr.sin_port = htons(configuration->port);
   	server_addr.sin_addr.s_addr = INADDR_ANY;
   	bzero(&(server_addr.sin_zero),8);
			
	pthread_t run;
	pthread_create(&run,NULL,oldMain,NULL);
	printf("My IP is: ");
	printIP(configuration->IP);
	printf("\n");
	if (configuration->gatewayMode == GATEWAY_ACCEPT) {
		setGateway(configuration->IP, configuration->mask);
	}
	return 0;
}

/* Function to handle any incoming packet on the network */
void packetArrival(packet * packet1) {
	if(memcmp(packet1->source, configuration->IP, 4)) {	
		if(packet1->type == TYPE_BEACON) {
			send_all(inet_ntoa(client_addr.sin_addr));
				//memset(packet1,NULL,sizeof(packet));
			free(packet1);
		} else if(packet1->type == TYPE_DATA) {
//printf("NEW PACKET ARRIVAL seq: %d type: %d\n", packet1->seq_num, packet1->type);
		
	data_handler(packet1);
		} else if(packet1->type == TYPE_ACK) {
			ack_handler(packet1);
		}
	}
}

/* Function called to by the reciving thread to start the reception */
void *waitForPacket() {
	int bytes_read;		
	int addr_len = sizeof(struct sockaddr);		
		
    if (bind(MainSocket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) {
       	perror("Bind");
       	exit(1);
    }
		
	while(1) {
		packet *packet1;
		packet1 = (packet *)malloc(sizeof(packet));
		bytes_read = recvfrom(MainSocket,packet1,sizeof(packet),0,(struct sockaddr *)&client_addr, &addr_len);
		packetArrival(packet1);
	}
}


/* Function called to by other functions to send the packets */
void sendPacket(struct Apacket *packet, char *ip) {
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(8000);
	bzero(&(server_addr.sin_zero),8);
	sendto(MainSocket, (char *)packet, packet->length, 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
}

/* Function called by timer to send beacons periodically */
void send_beacon() {
	//printf("Beacon\n");
	packet * beacon = (packet *)malloc(sizeof(packet));
	beacon->type = TYPE_BEACON;
	beacon->seq_num = 0;
	sprintf(beacon->data,"Beacon\n");
	beacon->source[0]=configuration->IP[0];
	beacon->source[1]=configuration->IP[1];
	beacon->source[2]=configuration->IP[2];
	beacon->source[3]=configuration->IP[3];
	beacon->length = sizeof(packet)-MAX_DATA_SIZE+strlen(beacon->data);
	char buf[20];
	int i=0, j=0;
	for (i=0; i<configuration->numberBroadcasts; i++) {
		sprintf(buf, "%d.%d.%d.%d", configuration->broadcastIP[j+0]>=0?configuration->broadcastIP[j+0]:configuration->broadcastIP[j+0]+256, configuration->broadcastIP[j+1]>=0?configuration->broadcastIP[j+1]:configuration->broadcastIP[j+1]+256, configuration->broadcastIP[j+2]>=0?configuration->broadcastIP[j+2]:configuration->broadcastIP[j+2]+256, configuration->broadcastIP[j+3]>=0?configuration->broadcastIP[j+3]:configuration->broadcastIP[j+3]+256);
		sendPacket(beacon, buf);
		j=j+4;
	}
	free(beacon);
}

/* Function called by packetArrival on receiving a data packet */
void data_handler(packet *p) {
	packet *ack;
	int takePacket = 0;
	if(isOld(p)==NOT_OLD_PACKET) {
		takePacket = 0;
		if (gateway) {
			char pNet[4];
			pNet[0] = p->dest[0] & mask[0];
			pNet[1] = p->dest[1] & mask[1];
			pNet[2] = p->dest[2] & mask[2];
			pNet[3] = p->dest[3] & mask[3];
			if (pNet[0] != net[0] || pNet[1] != net[1] || pNet[2] != net[2] || pNet[3] != net[3]) {
				takePacket = 1;
			}
		}
		if(!memcmp(p->dest,configuration->IP,4) || takePacket==1) {//The received packet is destined for me.	
			ack=deliverPacket(p);	//Get the ACK from the bundle layer
			if(ack!=NULL) {
				isOld(ack);
				add_packetnode(ack);
				delete_packetnode(ack);
			}
		} else {
			add_packetnode(p);
		}
	}
	return;
}

/* Function called by packetArrival on receiving a ack packet */
void ack_handler(packet *p) {
	if(isOld(p)==NOT_OLD_PACKET) {
		//if(memcmp(p->dest,configuration->IP,4)!=0) {
			add_packetnode(p);
		//} else {
			//printf("ACK IS FOR ME... NOT ADDING TO LIST\n");
		//}
        delete_packetnode(p);
	}
	return;
}



