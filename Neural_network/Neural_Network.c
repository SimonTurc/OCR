#include "backprop.h"
#include "layer.h"
#include "neuron.h"
#include "mnist.h"

#define MAX_LENGHT 20
#define NUM_LAYERS 3
#define NUM_NEURON_0 784
#define NUM_NEURON_1 16
//#define NUM_NEURON_2 16
#define NUM_NEURON_3 10
#define ALPHA 0.15
#define NUM_TRAINING_EX 2
#define EPOCH 1

double randomWeight() // generate random weight between [0.0, 1.0]
	{
	    return (rand() / (double) RAND_MAX);
	}
#define Slope 1.0

layer *lay = NULL;
int num_layers;
int *num_neurons;
float *errors;
float full_cost;
float **input;
float *desired_outputs;
int n=1;

int main(void)
{
    int i;
    
    num_layers = NUM_LAYERS;

    num_neurons = (int*) malloc(num_layers * sizeof(int));
    memset(num_neurons,0,num_layers *sizeof(int));
    
    num_neurons[0] = NUM_NEURON_0 ;
    num_neurons[1] = NUM_NEURON_1 ;
    //num_neurons[2] = NUM_NEURON_2 ;
    num_neurons[2] = NUM_NEURON_3 ;

    // Initialize the neural network module
    if(init()!= SUCCESS_INIT)
    {
        printf("Error in Initialization...\n");
        exit(0);
    }

    input = (float**) malloc(NUM_TRAINING_EX* sizeof(float*));
    for(i=0;i<NUM_TRAINING_EX;i++)
    {
        input[i] = (float*)malloc(num_neurons[0] * sizeof(float));
    }

    desired_outputs = (float*) malloc(NUM_TRAINING_EX* sizeof(float*));

    errors = (float *) malloc(num_neurons[num_layers-1] * sizeof(float));
    memset(errors,0,num_neurons[num_layers-1]*sizeof(float));

    // Get Training Examples
    //int inp[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    load_mnist();
    for (size_t i = 0; i < NUM_TRAINING_EX; i++)
    {
        Normalize_matrix(i);
    }
    get_inputs();

    // Get Output Labels
    //int out[4] = {0,1,1,0};
    get_desired_outputs();
    train_neural_net();
    //serialize();
    //deserialize("bot.txt");
    //test_nn();

    if(dinit()!= SUCCESS_DINIT)
    {
        printf("Error in Dinitialization...\n");
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
    return nodeOutput * (1- nodeOutput);
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
int init()
{
    if(create_architecture() != SUCCESS_CREATE_ARCHITECTURE)
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
    int i=0,j=0;
    lay = (layer*) malloc(num_layers * sizeof(layer));

    for(i=0;i<num_layers;i++)
    {
        lay[i] = create_layer(num_neurons[i]);      
        lay[i].num_neu = num_neurons[i];
        printf("Created Layer: %d\n", i+1);
        printf("Number of Neurons in Layer %d: %d\n", i,lay[i].num_neu);

        for(j=0;j<num_neurons[i];j++)
        {
            lay[i].neu[j] = create_neuron(num_neurons[i]);
        }
        printf("\n");
    }

    printf("\n");

    // Initialize the weights
    if(initialize_weights() != SUCCESS_INIT_WEIGHTS)
    {
        printf("Error Initilizing weights...\n");
        return ERR_CREATE_ARCHITECTURE;
    }

    return SUCCESS_CREATE_ARCHITECTURE;
}

int initialize_weights(void)
{
    int i,j,k;

    if(lay == NULL)
    {
        printf("No layers in Neural Network...\n");
        return ERR_INIT_WEIGHTS;
    }

    printf("Initializing weights...\n");

    for(i=0;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k< num_neurons[i+1];k++)
            {
                // Initialize Output Weights for each neuron
                lay[i].neu[j].out_weights[k] = randomWeight();
                printf("weight[%i][%i][%i] : %f\n" ,i,j,k,lay[i].neu[j].out_weights[k]);
            }

            if(i>0) 
            {
                lay[i].neu[j].bias = randomWeight();
                printf("bias[%i][%i] : %f \n" ,i,j,lay[i].neu[j].bias);
            }
        }
    }
    for (i = 0; i < num_neurons[num_layers-1]; i++)
    {
        lay[num_layers-1].neu[i].bias = randomWeight();
        printf("bias[%i][%i] : %f \n" ,num_layers-1,i,lay[num_layers-1].neu[i].bias);
    }
       
    printf("\n");
    return SUCCESS_INIT_WEIGHTS;
}

// Feed inputs to input layer
void feed_input(int i)
{
    int j;

    for(j=0;j<num_neurons[0];j++)
    {
        lay[0].neu[j].actv = input[i][j];
    }
    printf("Input: %d\n", train_label[i]);
    
}

void forward_prop(void)
{
    int i,j,k;
    for(i=1;i<num_layers;i++)
    {   
        for(j=0;j<num_neurons[i];j++)
        {
            double sum = lay[i].neu[j].bias;

            for(k=0;k< num_neurons[i-1];k++)
            {
                sum += lay[i-1].neu[k].out_weights[j] * lay[i-1].neu[k].actv;
            }
            double output = sigmoid(sum);
            lay[i].neu[j].actv = output;
            lay[i].neu[j].dbias = sigmoidDerivative(lay[i].neu[j].actv);
            printf("Output[%i][%i]: %f\n",i,j,lay[i].neu[j].actv);                
        }
    }
}

// Back Propogate Error
void back_prop(void)
{
    int i,j,k;
    // Output Layer
    for(j=0;j<num_neurons[num_layers-1];j++)
    {     
        lay[num_layers-1].neu[j].bias += (ALPHA*errors[j]* lay[num_layers-1].neu[j].dbias);
        for (k = 0; k < num_neurons[num_layers -2]; k++)
        {
            lay[num_layers-2].neu[k].out_weights[j] += lay[num_layers-2].neu[k].actv * ALPHA * errors[j];
        }
        
    }

    // Hidden Layers
    for(i=num_layers-2;i>0;i--)
    {
        double deltaHidden[num_neurons[i]];
        for(j=0;j<num_neurons[i];j++)
        {
            double hiddenError = 0.0 ;
            for(k=0;k<num_neurons[i+1];k++)
            {
                hiddenError += errors[k]*lay[i].neu[j].out_weights[k];
            }
            deltaHidden[j] = hiddenError * lay[i].neu[j].dbias;
            lay[i].neu[j].bias *= (ALPHA*deltaHidden[j]);
            if (i<num_layers-2)
            {
                for(k=0;k<num_neurons[i+1];k++)
                {
                    lay[i].neu[j].out_weights[k] += lay[i].neu[j].actv * ALPHA * deltaHidden[j];
                }
            }
        }
    }
}

// Calculate error between output of forward-prop and the desired_output
void calc_error(int i)
{
    for (size_t j = 0; j < num_neurons[num_layers-1]; j++)
    {
        errors[j] = (desired_outputs[i] - lay[num_layers-1].neu[j].actv);
    }
}

void update_weights(void)
{
    int i,j,k;

    for(i=1;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i-1];k++)
            {
                // Update Weights
                lay[i].neu[j].out_weights[k] += ALPHA * lay[i].neu[j].dbias * lay[i-1].neu[j].out_weights[k];
            }
            // Update Bias
            lay[i].neu[j].bias += (ALPHA * lay[i].neu[j].dbias);
        }
    } 
}

// Train Neural Network
void train_neural_net(void)
{
    int i;
    int it=0;

    // Gradient Descent
    for(it=0;it<EPOCH;it++)
    {
        for(i=0;i<NUM_TRAINING_EX;i++)
        {
            feed_input(i);
            forward_prop();
            calc_error(i);
            back_prop();
            //update_weights();
        }
        //printf("%i\n",it);
        printf("\n");
    }
}


// Serialize the network
void serialize(void)
{
    FILE *file = fopen("bot.txt","w+");
    fprintf(file,"%d\n",num_layers);
    for (size_t i = 0; i < num_layers ; i++)
    {
        fprintf(file,"%d\n",num_neurons[i]);
    }
    for (size_t i = 0; i < num_layers ; i++)
    {
        for(size_t j=0;j<num_neurons[i];j++)
        {
            fprintf(file,"%f\n",lay[i].neu[j].bias);
            for(size_t k = 0; k <num_neurons[i+1];k++)
            {
                fprintf(file,"%f\n",lay[i].neu[j].out_weights[k]);
            }
        }
    }
    printf("A bot.txt has been created\n");  
    fclose(file);
}

// Deserialize a bot.txt
void deserialize(char *bot)
{
    FILE *file = fopen(bot,"r");
    char str[MAX_LENGHT] = ""; 
    fgets(str,MAX_LENGHT, file);
    num_layers = atoi(str);
    num_neurons = (int*) malloc(num_layers * sizeof(int));
    for (size_t i = 0; i < num_layers; i++)
    {
        fgets(str,MAX_LENGHT, file);
        num_neurons[i] = atoi(str);
        printf("%d \n",num_neurons[i]);
    }
    init();
    for (size_t i = 0; i < num_layers; i++)
    {
        for (size_t j= 0; j < num_neurons[i]; j++)
        {
            fgets(str,MAX_LENGHT, file);
            lay[i].neu[j].bias = atof(str);
            for(size_t k = 0; k < num_neurons[i+1]; k++)
            {
                fgets(str,MAX_LENGHT, file);
                lay[i].neu[j].out_weights[k] = atof(str);
                printf("%ld:w[%ld][%ld]: %f\n",k,i,j, lay[i].neu[j].out_weights[k]);
            }
        }
    }
    printf("A neural network has been successfully created\n"); 
    fclose(file);
}

//Get Inputs
void get_inputs(void)
{
    int i,j;

        for(i=0;i<NUM_TRAINING_EX;i++)
        {
            for(j=0;j<num_neurons[0];j++)
            {
                input[i][j] = train_image[i][j];
                //printf("%1.f ",input[i][j]);
                //if ((j+1) % 28 == 0) putchar('\n');
            }
        }
}

//Get Labels
void get_desired_outputs(void)
{
    int i;
    
    for(i=0;i<NUM_TRAINING_EX;i++)
    {

        desired_outputs[i] = (float)train_label[i];
    }
}

void Normalize_matrix(int num_train_actual)
{
    for (int i=0; i<784; i++) 
    {
        if (train_image[num_train_actual][i] > 0.0)
        {
            train_image[num_train_actual][i] = 1.0;
        }
    }
}

// Test the trained network
void test_nn(void) 
{
    int i;
    int j = 0;
    while(j <5)
    {
        for(i=0;i<num_neurons[0];i++)
        {
            lay[0].neu[i].actv = train_image[j][i];
        }
        printf("Input: %i\n",train_label[j]);
        forward_prop();
        j++;
    }
}

int dinit(void)
{
    free(input);
    free(desired_outputs);
    free(num_neurons);
    free(lay);
    
    return SUCCESS_DINIT;
}
