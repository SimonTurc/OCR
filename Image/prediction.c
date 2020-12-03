#include "prediction.h"
#include "../Neural_network/layer.h"
#include "../Neural_network/neuron.h"

layer *lay = NULL;
int num_layers;
int *num_neurons;

int init(int num_layers, int *num_neurons)
{
    if (create_architecture(num_layers, num_neurons) != SUCCESS_CREATE_ARCHITECTURE)
    {
        printf("Error in creating architecture...\n");
        return ERR_INIT;
    }

    // printf("Neural Network Created Successfully...\n\n");
    return SUCCESS_INIT;
}

// Create Neural Network Architecture
int create_architecture(int num_layers, int *num_neurons)
{
    lay = (layer *)malloc(num_layers * sizeof(layer));

    for (int i = 0; i < num_layers; i++)
    {
        lay[i] = create_layer(num_neurons[i]);
        lay[i].num_neu = num_neurons[i];
        // printf("Created Layer: %d\n", i + 1);
        // printf("Number of Neurons in Layer %d: %d\n", i, lay[i].num_neu);

        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i < num_layers - 1)
            {
                lay[i].neu[j] = create_neuron(num_neurons[i + 1]);
            }
        }
        // printf("\n");
    }

    // printf("\n");

    return SUCCESS_CREATE_ARCHITECTURE;
}

char deserialize(double *matrix)
{
    FILE *file = fopen("Image/nn.txt", "r");
    if (file == NULL)
    {
        printf("couldn't open file ...\n");
        exit(-1);
    }
    char str[MAX_LENGHT];
    if (fgets(str, MAX_LENGHT, file) == NULL)
    {
        printf("Lost 1 ...\n");
        exit(-1);
    }
    int num_layers = atoi(str);
    int *num_neurons = (int *)malloc(num_layers * sizeof(int));
    for (int i = 0; i < num_layers; i++)
    {
        if (fgets(str, MAX_LENGHT, file) == NULL)
        {
            printf("Lost 2 ...\n");
            exit(-1);
        }
        num_neurons[i] = atoi(str);
        // printf("%d \n", num_neurons[i]);
    }
    init(num_layers, num_neurons);
    for (int i = 0; i < num_layers; i++)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (fgets(str, MAX_LENGHT, file) == NULL)
            {
                printf("Lost 3 ...\n");
                exit(-1);
            }
            lay[i].neu[j].bias = atof(str);
            if (i < num_layers - 1)
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    if (fgets(str, MAX_LENGHT, file) == NULL)
                    {
                        printf("Lost 4 ...\n");
                        exit(-1);
                    }
                    lay[i].neu[j].out_weights[k] = atof(str);
                    // printf("weight[%li][%li][%li] : %f\n", i, j, k, lay[i].neu[j].out_weights[k]);
                }
            }
        }
    }
    // printf("A neural network has been successfully created\n");
    fclose(file);
    for (int i = 0; i < num_neurons[0]; i++)
    {
        lay[0].neu[i].actv = matrix[i];
    }
    char letter = forward_prop_predict(num_layers, num_neurons);
    dinit(num_layers, num_neurons);
    return letter;
}

char forward_prop_predict(int num_layers, int *num_neurons)
{
    double max = 0;
    int result = 0;
    char letter;
    for (int i = 1; i < num_layers; i++)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            double sum = lay[i].neu[j].bias; // adding bias of the neuron

            for (int k = 0; k < num_neurons[i - 1]; k++)
            {
                sum += lay[i - 1].neu[k].out_weights[j] * lay[i - 1].neu[k].actv;
            }
            double output = sigmoid(sum);
            lay[i].neu[j].actv = output;
            lay[i].neu[j].grad = sigmoidDerivative(lay[i].neu[j].actv);
            // printf("Output[%i][%i]: %f\n", i, j, lay[i].neu[j].actv);
        }
    }
    for (int i = 0; i < num_neurons[num_layers - 1]; i++)
    {
        if (max < lay[num_layers - 1].neu[i].actv)
        {
            max = lay[num_layers - 1].neu[i].actv;
            result = i;
        }
    }
    if (result > 9)
    {
        letter = (char)(result - 9 + 65);
    }
    else
    {
        letter = (char)(result + 48);
    }

    return letter;
}

char predict(double *matrix)
{
    char charcter = deserialize(matrix);
    return charcter;
}

neuron create_neuron(int num_out_weights)
{
    neuron neu;

    neu.actv = 0.0;
    neu.out_weights = (double *)malloc(num_out_weights * sizeof(double));
    neu.bias = 0.0;
    neu.grad = 0.0;

    return neu;
}

layer create_layer(int number_of_neurons)
{
    layer lay;
    lay.num_neu = -1;
    lay.neu = (struct neuron_t *)malloc(number_of_neurons * sizeof(struct neuron_t));
    return lay;
}

double sigmoid(double value)
{
    return 1.0 / (1.0 + exp(-value));
}

double sigmoidDerivative(double nodeOutput)
{
    return nodeOutput * (1 - nodeOutput);
}

int dinit(int num_layers, int *num_neurons)
{
    for (int j = 0; j < num_layers; j++)
    {
        for (int k = 0; k < num_neurons[j]; k++)
        {
            if (j < num_layers - 1)
            {
                free(lay[j].neu[k].out_weights);
            }
        }
        free(lay[j].neu);
    }
    free(lay);
    free(num_neurons);
    return SUCCESS_DINIT;
}