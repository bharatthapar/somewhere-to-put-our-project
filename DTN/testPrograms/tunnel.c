/**
 * Creates a tunnel over the DTN
 *
 * Usage: ./tunnel <interface name to create> <path to config file>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../bundle.c"

#define IP_PACKET_SIZE 1500

#define IPV4 1
#define ICMP 2
#define DHCP 3

//The configuration file
extern config * configuration;

//File descriptor to the interface
int dtn0;

//Creates the tun interface
int tun_alloc(char * dev, int flags) {

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

//Creates the tun interface
int tun_init(char * name) {
	char intName[IFNAMSIZ];
	strncpy(intName, name, IFNAMSIZ-1);
	return tun_alloc(intName, IFF_TUN | IFF_NO_PI);
}

//Gets a packet from the interface
void getNetPacket() {
	int r;
	char buf9[MAX_DATA_SIZE];
	while (1) {
		//Read the packet from the interface
		r = read(dtn0, buf9, MAX_DATA_SIZE);

		//Send the packet as data over the DTN (&buf9[16] is the location of the IP address in the IP header)
		DTN_datasend(&buf9[16], buf9, r);
	}
}

//Write a packet to the interface
void injectNetPacket(int iface, packet * p) {
	char buf3[MAX_DATA_SIZE];
	while (1) {
		//Receive a packet from the DTN
		int r = DTN_datareceive(NULL, buf3, MAX_DATA_SIZE);
		//Put the packet on the interface
		write(dtn0, buf3, r);
	}
}

//Main function
int main(int argc, char * args[]) {
	sleep(7);

	createBundleLayer(args[2]);
	dtn0 = tun_init(args[1]);
	char buf7[200];
	
	//Configure the interface
	sprintf(buf7, "ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d up mtu %d", args[1], configuration->IP[0]>=0?configuration->IP[0]:configuration->IP[0]+256, configuration->IP[1]>=0?configuration->IP[1]:configuration->IP[1]+256, configuration->IP[2]>=0?configuration->IP[2]:configuration->IP[2]+256, configuration->IP[3]>=0?configuration->IP[3]:configuration->IP[3]+256, configuration->mask[0]>=0?configuration->mask[0]:configuration->mask[0]+256, configuration->mask[1]>=0?configuration->mask[1]:configuration->mask[1]+256, configuration->mask[2]>=0?configuration->mask[2]:configuration->mask[2]+256, configuration->mask[3]>=0?configuration->mask[3]:configuration->mask[3]+256, MAX_DATA_SIZE);
	system(buf7);
	sprintf(buf7, "tc qdisc add dev %s root handle 1: cbq avpkt 1000 bandwidth 250kbit", args[1]);
	system(buf7);
	
	//Start the threads
	pthread_t get,inject;
	pthread_create(&inject, NULL, injectNetPacket, NULL);
	pthread_create(&get, NULL, getNetPacket, NULL);
	
	while(1)
		pause();

}
