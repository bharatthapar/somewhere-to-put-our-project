all: sequence.o DTN.o queuestruc.o bundle.o data_Queue.o
	ar rcs DTN.a sequence.o DTN.o queuestruc.o bundle.o data_Queue.o
DTN.o: DTN.c
	gcc -c -lpthread -lrt -w DTN.c
queuestruc.o: queuestruc.c
	gcc -c -w queuestruc.c
sequence.o: sequence.c
	gcc -c -w sequence.c
bundle.o: bundle.c
	gcc -c -w bundle.c
data_Queue.o: data_Queue.c
	gcc -c -w data_Queue.c
clean:
	rm -rf *o final
