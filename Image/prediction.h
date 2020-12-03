#ifndef PREDICTION_H
#define PREDICTION_H

#include <math.h>

#define SUCCESS_INIT 0
#define ERR_INIT 1

#define SUCCESS_DINIT 0
#define ERR_DINIT 1

#define SUCCESS_INIT_WEIGHTS 0
#define ERR_INIT_WEIGHTS 1

#define SUCCESS_UPDATE_WEIGHTS 0

#define SUCCESS_CREATE_ARCHITECTURE 0
#define ERR_CREATE_ARCHITECTURE 1

#define MAX_LENGHT 50

char forward_prop_predict(int num_layers, int *num_neurons);
char predict(double *matrix);
char deserialize(double *matrix);
int create_architecture(int num_layers, int *num_neurons);
int init(int num_layers, int *num_neurons);
double sigmoid(double value);
double sigmoidDerivative(double nodeOutput);
int dinit(int num_layers, int *num_neurons);

#endif