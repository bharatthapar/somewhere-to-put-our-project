all: final

final: sequence.o UDP_server.o queuestruc.o bundle.o
	gcc -lpthread sequence.o UDP_server.o queuestruc.o bundle.o

UDP_server.o: UDP_server.c
	gcc -c -lpthread -lrt -w UDP_server.c
queuestruc.o: queuestruc.c
	gcc -c -w queuestruc.c
sequence.o: sequence.c
	gcc -c -w sequence.c
bundle.o: bundle.c
	gcc -c -w bundle.c

clean:
	rm -rf *o final
