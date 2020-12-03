CC =gcc -fsanitize=address
LDFLAGS = -pthread -lpthread
CFLAGS = -g -Wall -Werror 
backprop: backprop.o layer.o neuron.o
	$(CC) $(LDFLAGS) -o main Neural_Network.o layer.o neuron.o -lm

backprop.o: Neural_Network.c
	$(CC) $(CFLAGS) -c Neural_Network.c

layer.o: layer.c
	$(CC) $(CFLAGS) -c layer.c

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c

clean:
	${RM} *.o
	${RM} main
