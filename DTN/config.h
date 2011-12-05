#ifndef CONFIG_H
#define CONFIG_H

//Structure to hold the configuration options
typedef struct configOptions {
	char IP[4];
	char mask[4];
	int port;
	int gatewayMode;
	int packetLife;
	int beaconInterval;
	int numberBroadcasts;
	char * broadcastIP;
} config;

#define GATEWAY_OFF 1
#define GATEWAY_ACCEPT 2
#define GATEWAY_FORWARD 3
#define GATEWAY_RECEIVE 4

//Returns a config structure with the info from the given file
config * getConfiguration(char * sFile);

#endif
