#include "backprop.h"
// #include "dataset.h"
#include "datasetbis.h"
#include <err.h>

#define MAX_LENGHT 20
#define NUM_LAYERS 3
#define NUM_NEURON_0 784
#define NUM_NEURON_1 148
#define NUM_NEURON_3 83
#define ALPHA 0.15
#define NUM_TRAINING_EX 29 * 83
#define NUM_TEST_EX 83
#define EPOCH 150
#define Slope 1.0

layer *lay = NULL;
int num_layers;
int *num_neurons;
float *errors;
float full_cost;
double **input;
int *desired_outputs;
double success = 0;
int n = 1;

int main(void)
{
    int i;
    char buffer[200];
    char answer;
    char answer2;

    printf("Do you have a bot [y/n] : ");
    if (scanf("%c", &answer) == 0)
        errx(1, "Not enough memory !");
    if (answer == 'y')
    {
        printf("Do you have a bot [path] : ");
        if (scanf("%s", buffer) == 0)
            errx(1, "Not enough memory !");
        deserialize(buffer);
    }
    else
    {
        num_layers = NUM_LAYERS;
        num_neurons = (int *)malloc(num_layers * sizeof(int));
        memset(num_neurons, 0, num_layers * sizeof(int));

        num_neurons[0] = NUM_NEURON_0;
        num_neurons[1] = NUM_NEURON_1;
        num_neurons[2] = NUM_NEURON_3;

        // Initialize the neural network module
        if (init() != SUCCESS_INIT)
        {
            printf("Error in Initialization...\n");
            exit(0);
        }

        input = (double **)malloc(NUM_TRAINING_EX * sizeof(double *));
        for (i = 0; i < NUM_TRAINING_EX; i++)
        {
            input[i] = (double *)malloc(num_neurons[0] * sizeof(double));
        }

        desired_outputs = (int *)malloc(NUM_TRAINING_EX * sizeof(int));

        errors = (float *)malloc(num_neurons[num_layers - 1] * sizeof(float));
        memset(errors, 0, num_neurons[num_layers - 1] * sizeof(float));

        get_desired_outputs();
        train_neural_net();
        test_nn();

        printf("Do you want to serialize [y/n] : ");
        if (scanf("%c", &answer2) == 0)
            errx(1, "Not enough memory !");
        if (scanf("%c", &answer2) == 0)
            errx(1, "Not enough memory !");
        if (answer2 == 'y')
        {
            serialize();
        }
        if (dinit() != SUCCESS_DINIT)
        {
            printf("Error in Dinitialization...\n");
        }
    }

    return 0;
}
//----------------------------Activation function-----------------------------
static double sigmoid(double value)
{
    return 1.0 / (1.0 + exp(-value));
}

static double sigmoidDerivative(double nodeOutput)
{
    return nodeOutput * (1 - nodeOutput);
}

double softplus(double v)
{
    return log(1.0 + exp(Slope * v));
}

double d_softplus(double v)
{
    return Slope * 1.0 / (1.0 + exp(Slope * -v));
}

double x2(double v)
{
    return v * v + v;
}

double d_x2(double v)
{
    return 2.0 * v + 1.0;
}

//----------------------------Neural Network init---------------------------
double NormalDistribution()
{
    double a = (rand() / (double)RAND_MAX);
    double b = (rand() / (double)RAND_MAX);
    return sqrt(-2 * log(a)) * cos(2 * 3.14 * b) / 10;
}

double randomWeight() // generate random weight between [0.0, 1.0]
{
    return NormalDistribution();
}

int init()
{
    if (create_architecture() != SUCCESS_CREATE_ARCHITECTURE)
    {
        printf("Error in creating architecture...\n");
        return ERR_INIT;
    }

    printf("Neural Network Created Successfully...\n\n");
    return SUCCESS_INIT;
}

// Create Neural Network Architecture
int create_architecture()
{
    lay = (layer *)malloc(num_layers * sizeof(layer));

    for (int i = 0; i < num_layers; i++)
    {
        lay[i] = create_layer(num_neurons[i]);
        lay[i].num_neu = num_neurons[i];
        printf("Created Layer: %d\n", i + 1);
        printf("Number of Neurons in Layer %d: %d\n", i, lay[i].num_neu);

        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i < num_layers - 1)
            {
                lay[i].neu[j] = create_neuron(num_neurons[i + 1]);
            }
        }
        printf("\n");
    }

    printf("\n");

    // Initialize the weights
    if (initialize_weights() != SUCCESS_INIT_WEIGHTS)
    {
        printf("Error Initilizing weights...\n");
        return ERR_CREATE_ARCHITECTURE;
    }

    return SUCCESS_CREATE_ARCHITECTURE;
}

int initialize_weights(void)
{
    if (lay == NULL)
    {
        printf("No layers in Neural Network...\n");
        return ERR_INIT_WEIGHTS;
    }

    printf("Initializing weights...\n");

    for (int i = 0; i < num_layers - 1; i++)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            for (int k = 0; k < num_neurons[i + 1]; k++)
            {
                // Initialize Output Weights for each neuron
                lay[i].neu[j].out_weights[k] = randomWeight();
                printf("weight[%i][%i][%i] : %f\n", i, j, k, lay[i].neu[j].out_weights[k]);
            }

            if (i > 0)
            {
                lay[i].neu[j].bias = randomWeight();
                printf("bias[%i][%i] : %f \n", i, j, lay[i].neu[j].bias);
            }
        }
    }
    for (int i = 0; i < num_neurons[num_layers - 1]; i++)
    {
        lay[num_layers - 1].neu[i].bias = randomWeight();
        printf("bias[%i][%i] : %f \n", num_layers - 1, i, lay[num_layers - 1].neu[i].bias);
    }

    printf("\n");
    return SUCCESS_INIT_WEIGHTS;
}

// Feed inputs to input layer
void feed_input(int i)
{
    for (int j = 0; j < num_neurons[0]; j++)
    {
        lay[0].neu[j].actv = datasetbis[i][j];
    }
    printf("\n");
    if (i % 83 < 52)
    {
        printf("Input letter: %i\n", (int)dataset_label_letter[i % 83]);
    }
    else if (i % 83 < 73)
    {
        printf("Input special : %i\n", (int)dataset_label_spec[i % 83 - 52]);
    }
    else
    {
        printf("Input digit: %i\n", (int)dataset_label_digit[i % 83 - 73]);
    }
}

void forward_prop_train(int current_training)
{
    double max = 0;
    int result = 0;
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

    printf("Result : %d", result);
    if (result == (int)desired_outputs[current_training])
    {
        printf("\n");
    }
    else
    {
        printf(" XXX \n");
    }
}

char forward_prop_predict(int current_training)
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

    if (result == (int)desired_outputs[current_training])
    {
        success++;
        printf("Result : %d \n", result);
    }
    else
    {
        printf("Result : %d   XXX\n", result);
    }
    if (result < 26)
    {
        letter = (char)(result + 65);
    }
    else if (result < 52)
    {
        letter = (char)(result + 97 - 26);
    }
    else if (result < 53)
    {
        letter = (char)(result + 33 - 52);
    }
    else if (result < 66)
    {
        letter = (char)(result + 34 - 52);
    }
    else if (result < 73)
    {
        letter = (char)(result + 58 - 66);
    }
    else
    {
        letter = (char)(result + 48 - 73);
    }
    return letter;
}

// Back Propogate Error
void back_prop(void)
{

    double **hidden_error_matrix = (double **)malloc(num_layers * sizeof(double *)); // matrix for back propagate error
    for (int i = 0; i < num_layers; i++)
    {
        hidden_error_matrix[i] = (double *)malloc(num_neurons[i] * sizeof(double));
    }

    for (int i = num_layers - 1; i > 0; i--)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i == num_layers - 1)
            {
                hidden_error_matrix[i][j] = errors[j] * lay[i].neu[j].grad;
            }
            else
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    hidden_error_matrix[i][j] +=
                        hidden_error_matrix[i + 1][k] * lay[i].neu[j].out_weights[k] * lay[i].neu[j].grad;
                }
            }
        }
    }

    // Update weights and bias
    for (int i = num_layers - 1; i >= 0; i--)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i > 0)
            {
                lay[i].neu[j].bias -= (ALPHA * hidden_error_matrix[i][j]);
            }
            if (i < num_layers - 1)
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    lay[i].neu[j].out_weights[k] -= (ALPHA * hidden_error_matrix[i + 1][k] * lay[i].neu[j].actv);
                }
            }
        }
    }

    // Free the error matrix
    for (int i = 0; i < num_layers; i++)
    {
        free(hidden_error_matrix[i]);
    }
    free(hidden_error_matrix);
}

// Calculate error between output of forward-prop and the desired_output
void calc_error(int i)
{
    for (int j = 0; j < num_neurons[num_layers - 1]; j++)
    {
        if (desired_outputs[i] == j)
        {
            errors[j] = lay[num_layers - 1].neu[j].actv - 1;
        }
        else
        {
            errors[j] = lay[num_layers - 1].neu[j].actv;
        }
    }
}

// Train Neural Network
void train_neural_net(void)
{
    int i;
    int it = 0;

    // Gradient Descent
    for (it = 0; it < EPOCH; it++)
    {
        printf("------------------EPOCH : %i----------------------\n", it);
        // success = 0;
        for (i = 0; i < NUM_TRAINING_EX; i++)
        {
            feed_input(i);
            forward_prop_train(i);
            calc_error(i);
            back_prop();
        }
        printf("\n");
    }
}

// Serialize the network
void serialize(void)
{
    FILE *file = fopen("bot.txt", "w+");
    fprintf(file, "%d\n", num_layers);
    for (int i = 0; i < num_layers; i++)
    {
        fprintf(file, "%d\n", num_neurons[i]);
    }
    for (int i = 0; i < num_layers; i++)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            fprintf(file, "%f\n", lay[i].neu[j].bias);
            if (i < num_layers - 1)
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    fprintf(file, "%f\n", lay[i].neu[j].out_weights[k]);
                }
            }
        }
    }
    printf("A bot.txt has been created\n");
    fclose(file);
}

// Init a architecture already existing
int init_exist(int num_layers, int *num_neurons)
{
    if (create_architecture_exist(num_layers, num_neurons) != SUCCESS_CREATE_ARCHITECTURE)
    {
        printf("Error in creating architecture...\n");
        return ERR_INIT;
    }
    return SUCCESS_INIT;
}

// Create Neural Network Architecture already existing
int create_architecture_exist(int num_layers, int *num_neurons)
{
    lay = (layer *)malloc(num_layers * sizeof(layer));

    for (int i = 0; i < num_layers; i++)
    {
        lay[i] = create_layer(num_neurons[i]);
        lay[i].num_neu = num_neurons[i];
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i < num_layers - 1)
            {
                lay[i].neu[j] = create_neuron(num_neurons[i + 1]);
            }
        }
    }

    return SUCCESS_CREATE_ARCHITECTURE;
}

// Deserialize a bot.txt
void deserialize(char *bot)
{
    FILE *file = fopen(bot, "r");
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
    }
    init_exist(num_layers, num_neurons);
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
                }
            }
        }
    }
    printf("A neural network has been successfully created\n");
    fclose(file);
    printf("%i\n", num_neurons[0]);
    input = (double **)malloc(NUM_TRAINING_EX * sizeof(double *));
    for (int i = 0; i < NUM_TRAINING_EX; i++)
    {
        input[i] = (double *)malloc(num_neurons[0] * sizeof(double));
    }

    desired_outputs = (int *)malloc(NUM_TRAINING_EX * sizeof(int));

    errors = (float *)malloc(num_neurons[num_layers - 1] * sizeof(float));
    memset(errors, 0, num_neurons[num_layers - 1] * sizeof(float));

    get_desired_outputs();
    train_neural_net_exist(num_layers, num_neurons);
    test_nn_exist(num_layers, num_neurons);
    char answer2;
    printf("Do you want to serialize [y/n] : ");
    if (scanf("%c", &answer2) == 0)
        errx(1, "Not enough memory !");
    if (scanf("%c", &answer2) == 0)
        errx(1, "Not enough memory !");
    if (answer2 == 'y')
    {
        serialize();
    }
    if (dinit_exist(num_layers, num_neurons) != SUCCESS_DINIT)
    {
        printf("Error in Dinitialization...\n");
    }
}

// Get Inputs
void get_inputs(void)
{
    int i, j;

    for (i = 0; i < NUM_TRAINING_EX; i++)
    {
        for (j = 0; j < num_neurons[0]; j++)
        {
            input[i][j] = train_image[i][j];
        }
    }
}

// Get Labels
void get_desired_outputs(void)
{
    int i;

    for (i = 0; i < NUM_TRAINING_EX; i++)
    {
        // desired_outputs[i] = (float)train_label[i];
        if (i % 83 < 52)
        {
            desired_outputs[i] = dataset_label_letter[i % 83];
        }
        else if (i % 83 < 73)
        {
            desired_outputs[i] = dataset_label_spec[i % 83 - 52];
        }
        else
        {
            desired_outputs[i] = dataset_label_digit[i % 83 - 73];
        }
    }
}

void Normalize_flip_matrix(int num_train_actual, double **flip_matrix)
{
    for (int i = 0; i < 784; i++)
    {
        if (num_train_actual % 5 == 0)
        {
            if (train_image[num_train_actual][i] > 0.5)
            {
                flip_matrix[num_train_actual][i] = 1.0;
            }
        }
        else if (train_image[num_train_actual][i] > 0.5)
        {
            flip_matrix[num_train_actual][(i % 28) * 28 + (i / 28)] = 1.0;
        }
    }
}

// Test the trained network
void test_nn(void)
{
    int i;
    success = 0;
    for (i = 0; i < NUM_TEST_EX; i++)
    {
        for (int j = 0; j < num_neurons[0]; j++)
        {
            lay[0].neu[j].actv = dataset_test[i][j];
        }
        if (i < 52)
        {
            printf("Input letter: %i\n", (int)dataset_label_letter[i]);
        }
        else if (i < 73)
        {
            printf("Input special : %i\n", (int)dataset_label_spec[i - 52]);
        }
        else
        {
            printf("Input digit: %i\n", (int)dataset_label_digit[i - 73]);
        }
        char letter = forward_prop_predict(i);
        printf("%c\n", letter);
    }
    printf("Accuracy : %f\n", success / NUM_TEST_EX);
}

int dinit(void)
{
    for (int i = 0; i < NUM_TRAINING_EX; i++)
    {
        free(input[i]);
    }
    free(input);
    free(desired_outputs);
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

// Feed inputs to input layer
void feed_input_exist(int i, int *num_neurons)
{
    for (int j = 0; j < num_neurons[0]; j++)
    {
        lay[0].neu[j].actv = datasetbis[i][j];
    }
    printf("\n");
    if (i % 83 < 52)
    {
        printf("Input letter: %i\n", (int)dataset_label_letter[i % 83]);
    }
    else if (i % 83 < 73)
    {
        printf("Input special : %i\n", (int)dataset_label_spec[i % 83 - 52]);
    }
    else
    {
        printf("Input digit: %i\n", (int)dataset_label_digit[i % 83 - 73]);
    }
}

void forward_prop_train_exist(int current_training, int num_layers, int *num_neurons)
{
    double max = 0;
    int result = 0;
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

    printf("Result : %d", result);
    if (result == (int)desired_outputs[current_training])
    {
        printf("\n");
    }
    else
    {
        printf(" XXX \n");
    }
}

char forward_prop_predict_exist(int current_training, int num_layers, int *num_neurons)
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

    if (result == (int)desired_outputs[current_training])
    {
        success++;
        printf("Result : %d \n", result);
    }
    else
    {
        printf("Result : %d   XXX\n", result);
    }
    if (result < 26)
    {
        letter = (char)(result + 65);
    }
    else if (result < 52)
    {
        letter = (char)(result + 97 - 26);
    }
    else if (result < 53)
    {
        letter = (char)(result + 33 - 52);
    }
    else if (result < 66)
    {
        letter = (char)(result + 34 - 52);
    }
    else if (result < 73)
    {
        letter = (char)(result + 58 - 66);
    }
    else
    {
        letter = (char)(result + 48 - 73);
    }
    return letter;
}

// Back Propogate Error
void back_prop_exist(int num_layers, int *num_neurons)
{

    double **hidden_error_matrix = (double **)malloc(num_layers * sizeof(double *)); // matrix for back propagate error
    for (int i = 0; i < num_layers; i++)
    {
        hidden_error_matrix[i] = (double *)malloc(num_neurons[i] * sizeof(double));
    }

    for (int i = num_layers - 1; i > 0; i--)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i == num_layers - 1)
            {
                hidden_error_matrix[i][j] = errors[j] * lay[i].neu[j].grad;
            }
            else
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    hidden_error_matrix[i][j] +=
                        hidden_error_matrix[i + 1][k] * lay[i].neu[j].out_weights[k] * lay[i].neu[j].grad;
                }
            }
        }
    }

    // Update weights and bias
    for (int i = num_layers - 1; i >= 0; i--)
    {
        for (int j = 0; j < num_neurons[i]; j++)
        {
            if (i > 0)
            {
                lay[i].neu[j].bias -= (ALPHA * hidden_error_matrix[i][j]);
            }
            if (i < num_layers - 1)
            {
                for (int k = 0; k < num_neurons[i + 1]; k++)
                {
                    lay[i].neu[j].out_weights[k] -= (ALPHA * hidden_error_matrix[i + 1][k] * lay[i].neu[j].actv);
                }
            }
        }
    }

    // Free the error matrix
    for (int i = 0; i < num_layers; i++)
    {
        free(hidden_error_matrix[i]);
    }
    free(hidden_error_matrix);
}

// Calculate error between output of forward-prop and the desired_output
void calc_error_exist(int i, int num_layers, int *num_neurons)
{
    for (int j = 0; j < num_neurons[num_layers - 1]; j++)
    {
        if (desired_outputs[i] == j)
        {
            errors[j] = lay[num_layers - 1].neu[j].actv - 1;
        }
        else
        {
            errors[j] = lay[num_layers - 1].neu[j].actv;
        }
    }
}

// Train Neural Network
void train_neural_net_exist(int num_layers, int *num_neurons)
{
    int i;
    int it = 0;

    // Gradient Descent
    for (it = 0; it < EPOCH; it++)
    {
        printf("------------------EPOCH : %i----------------------\n", it);
        // success = 0;
        for (i = 0; i < NUM_TRAINING_EX; i++)
        {
            feed_input_exist(i, num_neurons);
            forward_prop_train_exist(i, num_layers, num_neurons);
            calc_error_exist(i, num_layers, num_neurons);
            back_prop_exist(num_layers, num_neurons);
        }
        printf("\n");
    }
}

void test_nn_exist(int num_layers, int *num_neurons)
{
    int i;
    success = 0;
    for (i = 0; i < NUM_TEST_EX; i++)
    {
        for (int j = 0; j < num_neurons[0]; j++)
        {
            lay[0].neu[j].actv = dataset_test[i][j];
        }
        if (i < 52)
        {
            printf("Input letter: %i\n", (int)dataset_label_letter[i]);
        }
        else if (i < 73)
        {
            printf("Input special : %i\n", (int)dataset_label_spec[i - 52]);
        }
        else
        {
            printf("Input digit: %i\n", (int)dataset_label_digit[i - 73]);
        }
        char letter = forward_prop_predict_exist(i, num_layers, num_neurons);
        printf("%c\n", letter);
    }
    printf("Accuracy : %f\n", success / NUM_TEST_EX);
}

int dinit_exist(int num_layers, int *num_neurons)
{
    for (int i = 0; i < NUM_TRAINING_EX; i++)
    {
        free(input[i]);
    }
    free(input);
    free(desired_outputs);
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