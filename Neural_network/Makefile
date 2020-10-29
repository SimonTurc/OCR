LDFLAGS = -pthread -lpthread
CFLAGS = -g -Wall -Werror
backprop: backprop.o layer.o neuron.o
	$(CC) $(LDFLAGS) -o main Neural_network.o layer.o neuron.o -lm

backprop.o: Neural_network.c
	$(CC) $(CFLAGS) -c Neural_network.c

layer.o: layer.c
	$(CC) $(CFLAGS) -c layer.c

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c

clean:
	rm *.o backprop