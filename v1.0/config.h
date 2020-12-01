#ifndef CONFIG_H
#define CONFIG_H

#define MAX_LENGTH 20
#define N_LAYERS 3
#define ALPHA 0.15
#define N_TRAINING_EX 1
#define EPOCH 1000

static int N_NEURONS[3] = {784, 16, 10};

#endif

// 0 <= accuray = #success_with_test_dataset / #image_in_test_dataset <= 1
