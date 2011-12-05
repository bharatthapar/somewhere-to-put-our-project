all: sequence.o DTN.o forwardQueue.o bundle.o deliveryQueue.o config.o
	ar rcs DTN.a sequence.o DTN.o forwardQueue.o bundle.o deliveryQueue.o config.o
DTN.o: DTN.c packet.h forwardQueue.h sequence.h DTN.h bundle.h config.h forwardQueue.c sequence.c bundle.c config.c
	gcc -c -lpthread -lrt -w DTN.c
forwardQueue.o: forwardQueue.c forwardQueue.h packet.h DTN.h sequence.h DTN.c sequence.c
	gcc -c -w forwardQueue.c
sequence.o: sequence.c sequence.h packet.h
	gcc -c -w sequence.c
bundle.o: bundle.c bundle.h forwardQueue.h packet.h DTN.h sequence.h deliveryQueue.h config.h forwardQueue.c DTN.c sequence.c deliveryQueue.c config.c
	gcc -c -w bundle.c
deliveryQueue.o: deliveryQueue.c deliveryQueue.h
	gcc -c -w deliveryQueue.c
config.o: config.c config.h
	gcc -c -w config.c
clean:
	rm -rf sequence.o DTN.o forwardQueue.o bundle.o deliveryQueue.o config.o
	rm -rf DTN.a
