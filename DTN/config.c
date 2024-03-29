/**
 * Manages configuration options
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prints an error message and exits
void error(char * msg) {
	fprintf(stderr, "Error reading configuration file: %s\n", msg);
	exit(-1);
}

//Returns a config structure with the info from the given file
config * getConfiguration(char * sFile) {

	//Variables to make sure each element is specified
	int haveBundleAddress = 0;
	int haveBundleMask = 0;
	int havePort = 0;
	int haveBroadcastAddress = 0;
	int haveGatewayMode = 0;
	int havePacketLife = 0;
	int haveBeaconInterval = 0;

	config * out = malloc(sizeof(config));
	
	//Open the file
	FILE * file = fopen(sFile, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file %s\n", sFile);
		exit(-1);
	}
	
	char line1[256];
	char line2[256];
	int a;
	int b;
	int c;
	int d;
	
	//Read the parameters
	while (fgets(line1, 256, file) != NULL) {
		//Configuration options are preceeded with a "#"
		if (line1[0] == '#') {
			switch (line1[1]) {
				case 'B' :
					if (memcmp("#Bundle Address", line1, 15) == 0) {
						if (fscanf(file, "%d.%d.%d.%d", &a, &b, &c, &d) < 4)
							error("Could not read bundle address");
						out->IP[0] = a;
						out->IP[1] = b;
						out->IP[2] = c;
						out->IP[3] = d;
						haveBundleAddress = 1;	
					} else if (memcmp("#Bundle Netmask", line1, 15) == 0) {
						if (fscanf(file, "%d.%d.%d.%d", &a, &b, &c, &d) < 4)
							error("Could not read netmask");
						out->mask[0] = a;
						out->mask[1] = b;
						out->mask[2] = c;
						out->mask[3] = d;
						haveBundleMask = 1;
					} else if (memcmp("#Beacon Addresses", line1, 17) == 0) {
						out->numberBroadcasts = 0;
						int i = 0;
						haveBroadcastAddress = 1;
						while (fscanf(file, "%d.%d.%d.%d", &a, &b, &c, &d) >= 4) {
							line2[i] = a;
							line2[i+1] = b;
							line2[i+2] = c;
							line2[i+3] = d;
							i=i+4;
							out->numberBroadcasts++;
						}
						if (i == 0)
							error("Could not read beacon addresses");
						out->broadcastIP = malloc(i);
						memcpy(out->broadcastIP, line2, i);
					} else if (memcmp("#Beacon Interval", line1, 16) == 0) {
						if (fscanf(file, "%d", &(out->beaconInterval)) < 1)
							error("Could not read beacon interval");
						haveBeaconInterval = 1;	
					}
					break;
				case 'G':
					if (fgets(line2, 256, file) == NULL)
						error("Could not read gateway mode");
					haveGatewayMode = 1;
					switch (line2[0]) {
						case 'A':
							out->gatewayMode = GATEWAY_ACCEPT;
							break;
						case 'F':
							out->gatewayMode = GATEWAY_FORWARD;
							break;					
						case 'O':
							out->gatewayMode = GATEWAY_OFF;
							break;	
						case 'R':
							out->gatewayMode = GATEWAY_RECEIVE;
							break;
						default:
							haveGatewayMode = 0;
							error("Could not read gateway mode");
					}
					break;
				case 'P':
					if (memcmp("#Packet Life", line1, 12) == 0) {
						if (fscanf(file, "%d", &(out->packetLife)) < 1)
							error("Could not read packet life");
						havePacketLife = 1;					
					} else if (memcmp("#Port", line1, 5) == 0) {
						if (fscanf(file, "%d", &(out->port)) < 1)
							error("Could not read port");
						havePort = 1;
					}
					break;	
			}
		}
	}
	
	//Make sure a value was read for all the required elements
	if (haveBundleAddress == 0) {
		error("Bundle address not specified");
	}
	if (haveBundleMask == 0) {
		error("Bundle net mask not specified");
	}
	if (havePort == 0) {
		error("Port not specified");
	}
	if (haveBroadcastAddress == 0) {
		error("Beacon address(es) not specified");
	}
	if (haveGatewayMode == 0) {
		error("Gateway mode not specified");
	}
	if (havePacketLife == 0) {
		error("Packet life not specified");
	}
	if (haveBeaconInterval == 0) {
		error("Beacon interval not specified");
	}
	
	//Close the file
	fclose(file);
	
	//Return the loaded config structure
	return out;
}
