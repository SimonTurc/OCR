#ifndef BACKPROP_H
#define BACKPROP_H

#include "layer.h"
#include "mnist.h"

#define SUCCESS_INIT 0
#define ERR_INIT 1

#define SUCCESS_DINIT 0
#define ERR_DINIT 1

#define SUCCESS_INIT_WEIGHTS 0
#define ERR_INIT_WEIGHTS 1

#define SUCCESS_UPDATE_WEIGHTS 0

#define SUCCESS_CREATE_ARCHITECTURE 0
#define ERR_CREATE_ARCHITECTURE 1


int init(void);
int dinit(void);
int dinit_exist(int num_layers, int *num_neurons);

int create_architecture(void);
int initialize_weights(void);
void feed_input(int i);
void train_neural_net(void);
void forward_prop_train(int i);
char forward_prop_predict(int i);
void compute_cost(int i);
void back_prop(void);
void update_weights(void);
void get_inputs(void);
void get_desired_outputs(void);
void test_nn(void);
char predict(double *matrix);
void serialize(void);
int init_exist(int num_layers, int *num_neurons);
int create_architecture_exist(int num_layers, int *num_neurons);
void feed_input_exist(int i, int *num_neurons);
void forward_prop_train_exist(int current_training, int num_layers, int *num_neurons);
char forward_prop_predict_exist(int current_training, int num_layers, int *num_neurons);
void back_prop_exist(int num_layers, int *num_neurons);
void calc_error_exist(int i, int num_layers, int *num_neurons);
void train_neural_net_exist(int num_layers ,int *num_neurons);
void test_nn_exist(int num_layers,int *num_neurons);
void deserialize(char *bot);
void Normalize_flip_matrix(int num_train_actual, double **flip_matrix);
void calc_error(int i);
double randomWeight(void);

int initialize_dummy_weights(void);


#endif