#ifndef TUNNEL_H
#define TUNNEL_H

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "packet.h"
#include "sequence.h"
#include "UDP_Server.h"
#define IP_PACKET_SIZE 1500

#define IPV4 1
#define ICMP 2
#define DHCP 3



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

struct packet * getPacket(int iface){
	int r;
	char buf[IP_PACKET_SIZE];
	r = read(iface, buf, IP_PACKET_SIZE);
	
	//IP packet
	if ((buf[0] & 0xf0) == 0x40) {
		packet * p = malloc(sizeof(packet));
		if (p == NULL)
			return NULL;
		memcpy(p->source, &buf[12], 4);
		memcpy(p->dest, &buf[16], 4);
		//p->type = IPV4;
		//if (buf[9] == 1)
			//p->type = ICMP;
		addSequenceNumber(p);
		p->type = TYPE_DATA;
		p->ttl = DEFAULT_TTL;
		p->length = r + sizeof(packet) - MAX_FRAME_SIZE;
		memcpy(p->data, buf, r);
		return p;
	} else {
		return NULL;
	}
}

void sendPacket(int iface, packet * p) {
	write(iface, p->data, p->length - sizeof(packet) + MAX_FRAME_SIZE);
}

#endif