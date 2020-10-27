#include "mnist.h"

int main(void)
{
    // call to store mnist in array
    load_mnist();

    // print pixels of first data in test dataset
    int i;
    for (i=0; i<784; i++) {
        printf("%1.1f ", test_image[1][i]);
        if ((i+1) % 28 == 0) putchar('\n');
    }
    printf("\n");
    float image[28][28];
    for (i=0; i<784; i++) 
    {
        image[i / 28][i % 28] = test_image[0][i];
        if (image[i / 28][i % 28] > 0.0)
        {
            image[i / 28][i % 28] = 1.0;
        }
        printf("%1.1f ",image[i / 28][i % 28]);
        if ((i+1) % 28 == 0) putchar('\n');
    }

    // print first label in test dataset
    printf("label: %d\n", test_label[1]);

    // save image of first data in test dataset as .pgm file
    //save_mnist_pgm(test_image, 0);

    // show all pixels and labels in test dataset
    //print_mnist_pixel(test_image, NUM_TEST);
    //print_mnist_label(test_label, NUM_TEST);

    return 0;
}
