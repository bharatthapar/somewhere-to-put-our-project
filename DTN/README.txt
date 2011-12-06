The source files:
bundle.c - Contains functions that handle sending data to and receiving data from the user.
bundle.h - Contains function prototypes for bundle.c and should also be included when writing a user program.
config.c - Parses and verifies information from the config file used to initialize the bundle layer.
config.h - Function prototypes and structures for config.c.
deliveryQueue.c - Queue used for all packets that are destined for the node the code is running on.
deliveryQueue.h - Function prototypes and structures for deliveryQueue.c.
DTN.c - Main algorithm for the DTN routing protocol.
DTN.h - Function prototypes and structures for DTN.c.
forwardQueue.c - Queue used for all packets that are to be forwarded to others.
forwardQueue.h - Function prototypes and structures for forwardQueue.c.
packet.h - Structure and constants related to DTN packets.
sequence.c - Main algorithm for the DTN routing protocol.
sequence.h - Function prototypes and structures for sequence.c.

testPrograms/chat.c - Simple chat program using the DTN
testPrograms/file.c - Simple text file transfer program using the DTN
testPrograms/tunnel.c - Simple network tunnel using the DTN

To compile the DTN library:
type 'make' in the directory containing the source files

To compile one of the sample programs (in the same directory where make was executed) type:
gcc testPrograms/chat.c DTN.a -lpthread -ochat
gcc testPrograms/file.c DTN.a -lpthread -ofile
gcc testPrograms/tunnel.c DTN.a -lpthread -otunnel

The guides in /doc document how to execute these programs.
