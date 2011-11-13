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
#include"UDP_server.h"
#include"bundle.h"
#define INTERVAL 5000		/* number of milliseconds to go off */

int MainSocket;
int broadcast = 1;
struct sockaddr_in server_addr , client_addr;
char *my_ip[4],*temp;
char ipp[4];
void get_my_ip() {
	/* int fd;
	int a,b,c,d;
	 struct ifreq ifr;

	 fd = socket(AF_INET, SOCK_DGRAM, 0);
*/
	 /* I want to get an IPv4 IP address */
//	 ifr.ifr_addr.sa_family = AF_INET;

	 /* I want IP address attached to "eth0" */
//	 strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
	
//	 ioctl(fd, SIOCGIFADDR, &ifr);
	
//	 close(fd);
//	 char *temp=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
	//printf("\n\n\n%s\n\n\n",my_ip);
	//strcpy(my_ip,temp);
//	sscanf(temp,my_ip
	//inet_aton(temp,(struct in_addr*)my_ip); 
/*	sscanf(temp,"%u.%u.%u.%u",&a,&b,&c,&d);
	

	my_ip[0] = a+256;
	my_ip[1] = b;
	my_ip[2] = c;
	my_ip[3] = d;



	if (my_ip[0]<-1)
		my_ip[0]=my_ip[0]+256;
	if (my_ip[1]<0)
		my_ip[1]=my_ip[1]+256;
	if (my_ip[2]<0)
		my_ip[2]=my_ip[2]+256;
	if (my_ip[3]<0)
		my_ip[3]=my_ip[3]+256;

printf("IP is %d.%d.%d.%d\n",my_ip[0],my_ip[1],my_ip[2],my_ip[3]);
*/
char hostbuf[256];
       // char * ipbuf;
        struct hostent *hostentry;
        int ret;

        ret = gethostname(hostbuf,sizeof(hostbuf));

        if(-1 == ret){
                perror("gethostname");
                exit(1);
        }

        hostentry = gethostbyname(hostbuf);

        if(NULL == hostentry){
                perror("gethostbyname");
                exit(1);
        }
	
        temp = inet_ntoa(*((struct in_addr *)hostentry->h_addr_list[0]));

        if(NULL == my_ip){
                perror("inet_ntoa");
                exit(1);
        }
	int i =0;
	printf("%s\n",temp);
	my_ip[0] = (char*)strtok(temp,".");
		
		while (my_ip[i])
	 	{
			 i++;
	  		 my_ip[i] = strtok (NULL, ".");
			//itoa(my_ip[i],my_ip[i][0],10);
			printf("%s.....",my_ip[i]);
	  	}
		
	my_ip[0]=atoi(my_ip[0]);
	my_ip[1]=atoi(my_ip[1]);
my_ip[2]=atoi(my_ip[2]);
my_ip[3]=atoi(my_ip[3]);
	printf("IP is %d.%d.%d.%d",my_ip[0],my_ip[1],my_ip[2],my_ip[3]);
	
	
	ipp[0] = (int)my_ip[0];
	ipp[1] = (int)my_ip[1];
	ipp[2] = (int)my_ip[2];
	ipp[3] = (int)my_ip[3];

    //    printf("Hostname: %s Host IP: %s\n", hostbuf, my_ip);




}



int initialize() {
		struct in_addr a;
		char host[100];
		gethostname(host,sizeof(host));	
		
		printf("Host is %s\n\n",host);	
		struct hostent *name=gethostbyname(host);
		get_my_ip();
		//printf("IP is %s",my_ip[0]);
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
		
	        fflush(stdout);
		while(1) {
			
			bytes_read = recvfrom(listen_sock,packet1,1024,0,(struct sockaddr *)&client_addr, &addr_len);
	  		
			if(packet1->source[0]!=(my_ip[0]-256)||packet1->source[1]!=(my_ip[1]-256)||packet1->source[2]!=my_ip[2]||packet1->source[3]!=my_ip[3])
			{	//printf("\n\n\n%s\n\n\n",my_ip);
				printf("\n\n%d.%d.%d.%d\n\n",packet1->source[0],packet1->source[1],packet1->source[2],packet1->source[3]);
	          	printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	          	printf("\n%s\n", packet1->data);

			if(packet1->type == TYPE_BEACON) {
				printf("Received beacon from %s, sending all packets\n",inet_ntoa(client_addr.sin_addr));
				send_all(inet_ntoa(client_addr.sin_addr));
			}
			else if(packet1->type == TYPE_DATA) {
				data_handler(packet1);
			}
			else if(packet1->type == TYPE_ACK) {
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
	printf("IP is %d.%d.%d.%d\n",my_ip[0],my_ip[1],my_ip[2],my_ip[3]);	
	packet.source[0]=my_ip[0];
	packet.source[1]=my_ip[1];
	packet.source[2]=my_ip[2];
	packet.source[3]=my_ip[3];
	printf("%d.%d.%d.%d\n",packet.source[0]+256,packet.source[1]+256,packet.source[2],packet.source[3]);
	packet.length = sizeof(packet)-MAX_FRAME_SIZE+strlen(packet.data);
	char dest[4]={192,168,1,124};
	sendPackets(&packet,"192.168.1.255");
	
	newPacket(ipp,dest);
}

void data_handler(struct Apacket *packet) {
	/*	Got packet ---- Update seq num ---- If 1 -> store data in linked list, else return */
	struct Apacket *ack;
	if(isOld(packet)==NOT_OLD_PACKET) {
		if(!memcmp(packet->dest,ipp,4))	//The received packet is destined for me.
		{	printf("Hello Hello");	
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



