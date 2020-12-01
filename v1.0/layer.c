#include "layer.h"

layer create_layer(int n_neurons)
{
    layer layer;
    layer.size = -1;
    layer.neurons = (struct neuron_t *)malloc(n_neurons * sizeof(struct neuron_t));
    return layer;
}
