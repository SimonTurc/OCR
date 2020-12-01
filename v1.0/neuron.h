#ifndef NEURON_H
#define NEURON_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct neuron_t
{
    double value;
    double *out_weights;
    double bias;
    double grad;

} neuron;

neuron create_neuron(int size_out_weights);

#endif
