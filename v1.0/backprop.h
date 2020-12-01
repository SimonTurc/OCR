#ifndef BACKPROP_H
#define BACKPROP_H

#include "layer.h"

#define SUCCESS_INIT 0
#define ERR_INIT 1

#define SUCCESS_DINIT 0
#define ERR_DINIT 1

#define SUCCESS_INIT_WEIGHTS 0
#define ERR_INIT_WEIGHTS 1

#define SUCCESS_UPDATE_WEIGHTS 0

#define SUCCESS_CREATE_ARCHITECTURE 0
#define ERR_CREATE_ARCHITECTURE 1

static double sigmoid(double value)
{
    return 1.0 / (1.0 + exp(-value));
}

static double sigmoidDerivative(double nodeOutput)
{
    return nodeOutput * (1 - nodeOutput);
}

// ---------------------
// Train
// ---------------------
void train();

// Init
void create_architecture(int n_layers, int *n_neurons, layer *layers);
void initialize_weights(int n_layers, int *n_neurons, layer *layers);
double randomWeight();

// Training phase
void feed_input(int *n_neurons, layer *layers, double *training_image);
void forward_prop(int n_layers, int *n_neurons, layer *layers);
void calc_error(int n_layers, int *n_neurons, layer *layers, int training_labels, double *errors);
void back_prop(int n_layers, int *n_neurons, layer *layers, double *errors);

// Predict
void predict();

#endif
