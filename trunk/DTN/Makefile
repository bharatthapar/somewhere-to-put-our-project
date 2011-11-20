all: final

final: sequence.o UDP_server.o queuestruc.o bundle.o data_Queue.o api.o
	ar rcs DTN.a sequence.o UDP_server.o queuestruc.o bundle.o data_Queue.o api.o
UDP_server.o: UDP_server.c
	gcc -c -lpthread -lrt -w UDP_server.c
queuestruc.o: queuestruc.c
	gcc -c -w queuestruc.c
sequence.o: sequence.c
	gcc -c -w sequence.c
bundle.o: bundle.c
	gcc -c -w bundle.c
data_Queue.o: data_Queue.c
	gcc -c -w data_Queue.c
api.o: api.c
	gcc -c -w api.c	
clean:
	rm -rf *o final
