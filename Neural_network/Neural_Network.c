#include "backprop.h"
#include "layer.h"
#include "neuron.h"
#include "mnist.h"

#define MAX_LENGHT 20
#define NUM_LAYERS 4
#define NUM_NEURON_0 784
#define NUM_NEURON_1 1664
#define NUM_NEURON_2 1024
#define NUM_NEURON_3 1
#define ALPHA 0.15
#define NUM_TRAINING_EX 500

layer *lay = NULL;
int num_layers;
int *num_neurons;
float alpha;
float *cost;
float full_cost;
float **input;
float **desired_outputs;
int num_training_ex;
int n=1;

int main(void)
{
    int i;
    
    //printf("Enter the number of Layers in Neural Network:\n");
    //scanf("%d",&num_layers);
    num_layers = NUM_LAYERS;

    num_neurons = (int*) malloc(num_layers * sizeof(int));
    memset(num_neurons,0,num_layers *sizeof(int));

    // Get number of neurons per layer
    /*for(i=0;i<num_layers;i++)
    {
        printf("Enter number of neurons in layer[%d]: \n",i+1);
        scanf("%d",&num_neurons[i]);
    }*/
    
    num_neurons[0] = NUM_NEURON_0 ;
    num_neurons[1] = NUM_NEURON_1 ;
    num_neurons[2] = NUM_NEURON_2 ;
    num_neurons[3] = NUM_NEURON_3 ;

    // Initialize the neural network module
    if(init()!= SUCCESS_INIT)
    {
        printf("Error in Initialization...\n");
        exit(0);
    }

    //printf("Enter the learning rate (Usually 0.15): \n");
    //scanf("%f",&alpha);

    alpha = ALPHA;

    //printf("Enter the number of training examples: \n");
    //scanf("%d",&num_training_ex);

    num_training_ex = NUM_TRAINING_EX ;

    input = (float**) malloc(num_training_ex * sizeof(float*));
    for(i=0;i<num_training_ex;i++)
    {
        input[i] = (float*)malloc(num_neurons[0] * sizeof(float));
    }

    desired_outputs = (float**) malloc(num_training_ex* sizeof(float*));
    for(i=0;i<num_training_ex;i++)
    {
        desired_outputs[i] = (float*)malloc(num_neurons[num_layers-1] * sizeof(float));
    }

    //cost = (float *) malloc(num_neurons[num_layers-1] * sizeof(float));
    //memset(cost,0,num_neurons[num_layers-1]*sizeof(float));

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
    printf("images get\n\n");
    train_neural_net();
    serialize();
    //deserialize("bot.txt");
    test_nn();
    
    if(dinit()!= SUCCESS_DINIT)
    {
        printf("Error in Dinitialization...\n");
    }

    return 0;
}


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

//Get Inputs
void get_inputs(void)
{
    int i,j;

        for(i=0;i<num_training_ex;i++)
        {
            //printf("Enter the Inputs for training example[%d]:\n",i);

            for(j=0;j<num_neurons[0];j++)
            {
                //scanf("%f",&input[i][j]);
                input[i][j] = train_image[i][j];
                
            }
            //printf("\n");
        }
}

//Get Labels
void get_desired_outputs(void)
{
    int i,j;
    
    for(i=0;i<num_training_ex;i++)
    {
        for(j=0;j<num_neurons[num_layers-1];j++)
        {
            /*printf("Enter the Desired Outputs (Labels) for training example[%d]: \n",i);
            scanf("%f",&desired_outputs[i][j]);
            printf("\n");*/
            desired_outputs[i][j] = train_label[i];
        }
    }
}

// Feed inputs to input layer
void feed_input(int i)
{
    int j;

    for(j=0;j<num_neurons[0];j++)
    {
        lay[0].neu[j].actv = input[i][j];
        //printf("Input: %f\n",lay[0].neu[j].actv);
    }
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
        printf("Number of Neurons in Layer %d: %d\n", i+1,lay[i].num_neu);

        for(j=0;j<num_neurons[i];j++)
        {
            if(i < (num_layers-1)) 
            {
                lay[i].neu[j] = create_neuron(num_neurons[i+1]);
            }

            printf("Neuron %d in Layer %d created\n",j+1,i+1);  
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
            for(k=0;k<num_neurons[i+1];k++)
            {
                // Initialize Output Weights for each neuron
                lay[i].neu[j].out_weights[k] = ((double)rand())/((double)RAND_MAX);
                //printf("%d:w[%d][%d]: %f\n",k,i,j, lay[i].neu[j].out_weights[k]);
                lay[i].neu[j].dw[k] = 0.0;
            }

            if(i>0) 
            {
                lay[i].neu[j].bias = ((double)rand())/((double)RAND_MAX);
            }
        }
    }   
    printf("\n");
    lay[num_layers-1].neu[num_layers-1].bias = ((double)rand())/((double)RAND_MAX);

    return SUCCESS_INIT_WEIGHTS;
}

// Train Neural Network
void train_neural_net(void)
{
    int i;
    int it=0;

    // Gradient Descent
    for(it=0;it<5;it++)
    {
        for(i=0;i<num_training_ex;i++)
        {
            feed_input(i);
            forward_prop();
            //compute_cost(i);
            back_prop(i);
            update_weights();
        }
        printf("%i\n",it);
    }
}

void update_weights(void)
{
    int i,j,k;

    for(i=0;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i+1];k++)
            {
                // Update Weights
                lay[i].neu[j].out_weights[k] = (lay[i].neu[j].out_weights[k]) - (alpha * lay[i].neu[j].dw[k]);
            }
            
            // Update Bias
            lay[i].neu[j].bias = lay[i].neu[j].bias - (alpha * lay[i].neu[j].dbias);
        }
    }   
}

void forward_prop(void)
{
    int i,j,k;

    for(i=1;i<num_layers;i++)
    {   
        for(j=0;j<num_neurons[i];j++)
        {
            lay[i].neu[j].z = lay[i].neu[j].bias;

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i].neu[j].z  = lay[i].neu[j].z + ((lay[i-1].neu[k].out_weights[j])* (lay[i-1].neu[k].actv));
            }

            // Relu Activation Function for Hidden Layers
            if(i < num_layers-1)
            {
                if((lay[i].neu[j].z) < 0)
                {
                    lay[i].neu[j].actv = 0;
                }

                else
                {
                    lay[i].neu[j].actv = lay[i].neu[j].z;
                }
            }
            
            // Sigmoid Activation function for Output Layer
            else
            {
                lay[i].neu[j].actv = 1/(1+exp(-lay[i].neu[j].z));
            }
        }
    }
}

// Compute Total Cost
void compute_cost(int i)
{
    int j;
    float tmpcost=0;
    float tcost=0;

    for(j=0;j<num_neurons[num_layers-1];j++)
    {
        tmpcost = desired_outputs[i][j] - lay[num_layers-1].neu[j].actv;
        cost[j] = (tmpcost * tmpcost)/2;
        tcost = tcost + cost[j];
    }   

    full_cost = (full_cost + tcost)/n;
    n++;
    // printf("Full Cost: %f\n",full_cost);
}

// Back Propogate Error
void back_prop(int p)
{
    int i,j,k;

    // Output Layer
    for(j=0;j<num_neurons[num_layers-1];j++)
    {           
        lay[num_layers-1].neu[j].dz = (lay[num_layers-1].neu[j].actv - desired_outputs[p][j]) * (lay[num_layers-1].neu[j].actv) * (1- lay[num_layers-1].neu[j].actv);

        for(k=0;k<num_neurons[num_layers-2];k++)
        {   
            lay[num_layers-2].neu[k].dw[j] = (lay[num_layers-1].neu[j].dz * lay[num_layers-2].neu[k].actv);
            lay[num_layers-2].neu[k].dactv = lay[num_layers-2].neu[k].out_weights[j] * lay[num_layers-1].neu[j].dz;
        }
            
        lay[num_layers-1].neu[j].dbias = lay[num_layers-1].neu[j].dz;           
    }

    // Hidden Layers
    for(i=num_layers-2;i>0;i--)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            if(lay[i].neu[j].z >= 0)
            {
                lay[i].neu[j].dz = lay[i].neu[j].dactv;
            }
            else
            {
                lay[i].neu[j].dz = 0;
            }

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i-1].neu[k].dw[j] = lay[i].neu[j].dz * lay[i-1].neu[k].actv;    
                
                if(i>1)
                {
                    lay[i-1].neu[k].dactv = lay[i-1].neu[k].out_weights[j] * lay[i].neu[j].dz;
                }
            }

            lay[i].neu[j].dbias = lay[i].neu[j].dz;
        }
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

void Normalize_matrix(int num_train_actual)
{
    for (int i=0; i<784; i++) 
    {
        if (train_image[num_train_actual][i] > 0.0)
        {
            train_image[num_train_actual][i] = 1.0;
        }
        //printf("%1.1f ",train_image[num_train_actual][i]);
        //if ((i+1) % 28 == 0) putchar('\n');
    }
}

// Test the trained network
void test_nn(void) 
{
    int i;
    int j = 0;
    while(j <5)
    {
        //printf("Enter input to test:\n");

        for(i=0;i<num_neurons[0];i++)
        {
            lay[0].neu[i].actv = test_image[j][i];
        }
        forward_prop();
        printf("Output: %d ", (int)round(lay[num_layers-1].neu[0].actv));
        printf("wanted %d\n", test_label[j]);
        printf("\n");
        j++;
    }
}

int dinit(void)
{
    return SUCCESS_DINIT;
}



