#include "neuron.h"

neuron create_neuron(int size_out_weights)
{
    neuron neuron;

    neuron.value = 0.0;
    neuron.out_weights = (double *)malloc(size_out_weights * sizeof(double));
    neuron.bias = 0.0;
    neuron.grad = 0.0;

    return neuron;
}
