//May end up breaking this up into many files
#include "packet.h"

//Struct to store a packet along with other necessary info
struct PacketInfo {
	//The time after which the packet is no longer valid
	long timeout;
	//The time after whcih the packet should be sent again
	long sendTime;
	//The actual packet
	struct Packet * packet;
};

//Struct to store the latest known sequence numbers for hostss
struct Sequence {
	//The host address for whom this sequence number is associated
	char address[4];
	//The latest sequence number
	long seqNum;
	//The time at which this sequence number is no longer valid
	long timeout;
};

//Also need a data structure/function to manage outstanding packets
//and one to manage the sequence number records

//The main UDP socket for sending and receiving packets
int MainSocket;

//Initial set up including creating the main socket
void initialize() {


}

//Waits for a packet and returns it upon arrival (this will most likely be a thread)
struct Packet * waitForPacket() {

}

//Function called when new data is generated to send on the network
void newData(char * data) {

}

//Sends an ACK to ALL for the given packet
//Used when this is the packet's destination
void sendACK(struct Packet * p) {

}

//Sends an ACK to the previous hop
//Used after arrival of any packet
void sendLinkACK(struct Packet * p) {

}

//Purges packets that have timed out from storage
void purgeOldPackets() {

}

//Sends any stored packets that are ready to be (re)transmitted
void sendPackets() {

}

//You know what this is for :)
void main(int argc, char *argv[]){
	initialize();
}