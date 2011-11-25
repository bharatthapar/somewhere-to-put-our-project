#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "packet.h"
#include "sequence.h"
#include "DTN.h"
#include "config.h"
#define IP_PACKET_SIZE 1500

#define IPV4 1
#define ICMP 2
#define DHCP 3

extern config * configuration;
int dtn0;

int tun_alloc(char *dev, int flags) {

	struct ifreq ifr;
	int fd, err;

	if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
		perror("Opening /dev/net/tun");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = flags;

	if (*dev) {
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	if( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 ) {
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}

	strcpy(dev, ifr.ifr_name);

	return fd;
}

int tun_init(char * name) {
	char intName[IFNAMSIZ];
	strncpy(intName, name, IFNAMSIZ-1);
	return tun_alloc(intName, IFF_TUN | IFF_NO_PI);
}
void getNetPacket(){
	int r;
	char buf9[MAX_DATA_SIZE];
	while (1) {

		r = read(dtn0, buf9, MAX_DATA_SIZE);
		printf("Sent %d\n", r);
		DTN_datasend(&buf9[16], buf9, r);

	}
	//IP packet
	//if ((buf[0] & 0xf0) == 0x40) {
	//	packet * p = malloc(sizeof(packet));
	//	if (p == NULL)
	//		return NULL;
	//	memcpy(p->source, &buf[12], 4);
	//	memcpy(p->dest, &buf[16], 4);
		//p->type = IPV4;
		//if (buf[9] == 1)
			//p->type = ICMP;
	//	addSequenceNumber(p);
	//	p->type = TYPE_DATA;
	//	p->ttl = DEFAULT_TTL;
	//	p->length = r + sizeof(packet) - MAX_DATA_SIZE;
	//	memcpy(p->data, buf, r);
	//	return p;
	//} else {
	//	return NULL;
	//}
}



void injectNetPacket(int iface, packet * p) {
	char buf3[MAX_DATA_SIZE];
	while (1) {
		int r = DTN_datareceive(NULL, buf3, MAX_DATA_SIZE);
		int w = write(dtn0, buf3, r);
		printf("Size rec %d and written %d\n", r, w);
		
	}
}

int main(int argc, char * args[]) {
	createBundleLayer(args[2]);
	dtn0 = tun_init(args[1]);
	char buf7[200];
	sprintf(buf7, "ifconfig %s %d.%d.%d.%d/24 up mtu %d",args[1] , configuration->IP[0]>=0?configuration->IP[0]:configuration->IP[0]+256, configuration->IP[1]>=0?configuration->IP[1]:configuration->IP[1]+256, configuration->IP[2]>=0?configuration->IP[2]:configuration->IP[2]+256, configuration->IP[3]>=0?configuration->IP[3]:configuration->IP[3]+256, MAX_DATA_SIZE);
	system(buf7);
	pthread_t sender,receiver;
	pthread_create(&receiver,NULL,injectNetPacket,NULL);
	pthread_create(&receiver,NULL,getNetPacket,NULL);
	
	while(1);

}
