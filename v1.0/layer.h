#ifndef LAYER_H
#define LAYER_H

#include "neuron.h"

typedef struct layer_t
{
    int size;
    struct neuron_t *neurons;
} layer;

layer create_layer(int n_neurons);

#endif
