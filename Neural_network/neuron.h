#ifndef NEURON_H
#define NEURON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct neuron_t
{
	double actv;
	double *out_weights;
	double bias;
	double z;

	double dactv;
	double *dw;
	double dbias;
	double dz;

} neuron;

neuron create_neuron(int num_out_weights);

#endif