#include "backprop.h"
#include "config.h"
#include "layer.h"
#include "mnist.h"
#include "neuron.h"

// argv[1] == train or predict
// if predict then argv[2] is the path to the file
int main(int argc, char const *argv[])
{
    if (argc == 2 && !strcmp(argv[1], "train"))
    {
        train();
    }
    else if (argc == 3 && !strcmp(argv[1], "predict"))
    {
        // argv[1] holds the path
        predict(argv[2]);
    }
    return 0;
}

// -----------------------------------------
// Train
// ----------------------------------------

void train()
{
    // Define variables
    int n_layers = N_LAYERS;
    int *n_neurons = N_NEURONS;
    layer *layers = (layer *)malloc(n_layers * sizeof(layer));
    double **training_dataset = (double **)malloc(NUM_TRAIN * sizeof(double *));
    for (int i = 0; i < NUM_TRAIN; i++)
    {
        training_dataset[i] = (double *)malloc(SIZE * sizeof(double));
    }
    int *training_labels = (int *)malloc(NUM_TRAIN * sizeof(int));

    // Initalize model
    create_architecture(n_layers, n_neurons, layers);
    initialize_weights(n_layers, n_neurons, layers);
    printf("Initilization done !\n");

    // Load datasets
    load_mnist_train(training_dataset, training_labels);

    // Training phase
    double *errors = (double *)malloc(n_neurons[n_layers - 1] * sizeof(double));
    for (int epoch = 0; epoch < EPOCH; epoch++)
    {
        for (int training_index = 0; training_index < N_TRAINING_EX; training_index++)
        {
            feed_input(n_neurons, layers, training_dataset[training_index]);
            forward_prop(n_layers, n_neurons, layers);
            calc_error(n_layers, n_neurons, layers, training_labels[training_index], errors);
            back_prop(n_layers, n_neurons, layers, errors);
        }
    }

    return;
}

// -----------------------------------------
// Init
// ----------------------------------------

// Create Neural Network Architecture
void create_architecture(int n_layers, int *n_neurons, layer *layers)
{
    for (int i = 0; i < n_layers; i++)
    {
        layers[i] = create_layer(n_neurons[i]);
        layers[i].size = n_neurons[i];

        for (int j = 0; j < n_neurons[i]; j++)
        {
            layers[i].neurons[j] = create_neuron(n_neurons[i]);
        }
    }
    return;
}

void initialize_weights(int n_layers, int *n_neurons, layer *layers)
{
    // Init weights and bias for all layers except the last one
    for (int i = 0; i < n_layers - 1; i++)
    {
        for (int j = 0; j < n_neurons[i]; j++)
        {
            for (int k = 0; k < n_neurons[i + 1]; k++)
            {
                // Initialize Output Weights for each neuron
                layers[i].neurons[j].out_weights[k] = randomWeight();
            }

            if (i > 0)
            {
                layers[i].neurons[j].bias = randomWeight();
            }
        }
    }

    // Init bias for the last layer
    for (int j = 0; j < n_neurons[n_layers - 1]; j++)
    {
        layers[n_layers - 1].neurons[j].bias = randomWeight();
    }

    return;
}

double randomWeight() // generate random weight between following the normal distribution
{
    double a = (rand() / (double)RAND_MAX);
    double b = (rand() / (double)RAND_MAX);
    return sqrt(-2 * log(a)) * cos(2 * 3.14 * b) / 10;
}

// -----------------------------------------
// Training phase
// -----------------------------------------

// Feed inputs to input layer
void feed_input(int *n_neurons, layer *layers, double *training_image)
{
    for (int j = 0; j < n_neurons[0]; j++)
    {
        layers[0].neurons[j].value = training_image[j];
    }
}

void forward_prop(int n_layers, int *n_neurons, layer *layers)
{
    for (int i = 1; i < n_layers; i++)
    {
        for (int j = 0; j < n_neurons[i]; j++)
        {
            double sum = layers[i].neurons[j].bias; // adding bias of the neuron

            for (int k = 0; k < n_neurons[i - 1]; k++)
            {
                sum +=
                    layers[i - 1].neurons[k].out_weights[j] * layers[i - 1].neurons[k].value;
            }
            double output = sigmoid(sum);
            layers[i].neurons[j].value = output;
            layers[i].neurons[j].grad = sigmoidDerivative(layers[i].neurons[j].value);
            printf("Output[%i][%i]: %f\n", i, j, layers[i].neurons[j].value);
        }
    }
}

// Calculate error between output of forward-prop and the desired_output
void calc_error(int n_layers, int *n_neurons, layer *layers, int training_labels, double *errors)
{
    for (int j = 0; j < n_neurons[n_layers - 1]; j++)
    {
        if (training_labels == j)
        {
            errors[j] = layers[n_layers - 1].neurons[j].value - 1;
        }
        else
        {
            errors[j] = layers[n_layers - 1].neurons[j].value;
        }
    }

    return;
}

// Back Propogate Error
void back_prop(int n_layers, int *n_neurons, layer *layers, double *errors)
{
    double **error_matrix = (double **)malloc((n_layers) * sizeof(double));
    for (int i = 0; i < n_layers; i++)
    {
        error_matrix[i] = (double *)malloc(n_neurons[i] * sizeof(double));
    }

    // Compute the error matrix
    for (int i = n_layers - 1; i > 0; i--)
    {
        for (int j = 0; j < n_neurons[i]; j++)
        {
            // Compute error
            if (i == n_layers - 1)
            {
                error_matrix[i][j] = errors[j] * layers[i].neurons[j].grad;
            }
            else
            {
                error_matrix[i][j] = 0.;
                for (int k = 0; k < n_neurons[i + 1]; k++)
                {
                    error_matrix[i][j] += error_matrix[i + 1][k] * layers[i].neurons[j].out_weights[k] * layers[i].neurons[j].grad;
                }
            }
            // printf("error_matrix[%i][%i]: %f\n", i, j, error_matrix[i][j]);
        }
    }

    // Update weights
    for (int i = n_layers - 2; i >= 0; i--)
    {
        for (int j = 0; j < n_neurons[i]; j++)
        {
            for (int k = 0; k < n_neurons[i + 1]; k++)
            {
                layers[i].neurons[j].out_weights[k] -= (ALPHA * error_matrix[i + 1][k] * layers[i].neurons[j].value);
            }
        }
    }

    // Update bias
    for (int i = n_layers - 1; i > 0; i--)
    {
        for (int j = 0; j < n_neurons[i]; j++)
        {
            layers[i].neurons[j].bias -= (ALPHA * error_matrix[i][j]);
        }
    }

    for (int i = 0; i < n_layers - 1; i++)
    {
        free(error_matrix[i]);
    }
    free(error_matrix);
}

// -----------------------------------------
// Predict
// ----------------------------------------

void predict()
{
    return;
}
