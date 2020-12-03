/*
Takafumi Hoiruchi. 2018.
https://github.com/takafumihoriuchi/MNIST_for_C
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// set appropriate path for data
#define TRAIN_IMAGE_LETTRE "./data/emnist-letters-train-images-idx3-ubyte" 
#define TRAIN_LABEL_LETTRE "./data/emnist-letters-train-labels-idx1-ubyte" 
#define TEST_IMAGE_LETTRE  "./data/emnist-letters-test-images-idx3-ubyte" 
#define TEST_LABEL_LETTRE  "./data/emnist-letters-test-labels-idx1-ubyte" 

#define TRAIN_IMAGE "./data/train-images.idx3-ubyte"
#define TRAIN_LABEL "./data/train-labels.idx1-ubyte"
#define TEST_IMAGE  "./data/t10k-images.idx3-ubyte"
#define TEST_LABEL "./data/t10k-labels.idx1-ubyte"

#define SIZE 784 // 28*28
#define NUM_TRAIN 60000/4
#define NUM_TEST 10000/4
#define NUM_TRAIN_LETTRE 60000
#define NUM_TEST_LETTRE 10000
#define LEN_INFO_IMAGE 4
#define LEN_INFO_LABEL 2

#define MAX_IMAGESIZE 1280
#define MAX_BRIGHTNESS 255
#define MAX_FILENAME 256
#define MAX_NUM_OF_IMAGES 1

unsigned char image[MAX_NUM_OF_IMAGES][MAX_IMAGESIZE][MAX_IMAGESIZE];
int width[MAX_NUM_OF_IMAGES], height[MAX_NUM_OF_IMAGES];

int info_image[LEN_INFO_IMAGE];
int info_label[LEN_INFO_LABEL];

unsigned char train_image_char[NUM_TRAIN][SIZE];
unsigned char test_image_char[NUM_TEST][SIZE];
unsigned char train_label_char[NUM_TRAIN][1];
unsigned char test_label_char[NUM_TEST][1];

unsigned char train_image_char_letter[NUM_TRAIN_LETTRE][SIZE];
unsigned char test_image_char_letter[NUM_TEST_LETTRE][SIZE];
unsigned char train_label_char_letter[NUM_TRAIN_LETTRE][1];
unsigned char test_label_char_letter[NUM_TEST_LETTRE][1];

double train_image[NUM_TRAIN + NUM_TRAIN_LETTRE][SIZE];
double test_image[NUM_TEST + NUM_TEST_LETTRE][SIZE];
int  train_label[NUM_TRAIN + NUM_TRAIN_LETTRE];
int test_label[NUM_TEST+ NUM_TRAIN_LETTRE];


void FlipLong(unsigned char * ptr)
{
    register unsigned char val;
    
    // Swap 1st and 4th bytes
    val = *(ptr);
    *(ptr) = *(ptr+3);
    *(ptr+3) = val;
    
    // Swap 2nd and 3rd bytes
    ptr += 1;
    val = *(ptr);
    *(ptr) = *(ptr+1);
    *(ptr+1) = val;
}


void read_mnist_char(char *file_path, int num_data, int len_info, int arr_n, unsigned char data_char[][arr_n], int info_arr[])
{
    int i, fd;
    unsigned char *ptr;

    if ((fd = open(file_path, O_RDONLY)) == -1) {
        fprintf(stderr, "couldn't open image file \n");
        exit(-1);
    }
    
    if (read(fd, info_arr, len_info * sizeof(int)) == -1)
    {
        printf("pipo");
        exit(-1);
    }
    
    // read-in information about size of data
    for (i=0; i<len_info; i++) 
    { 
        ptr = (unsigned char *)(info_arr + i);
        FlipLong(ptr);
        ptr = ptr + sizeof(int);
    }
    
    // read-in mnist numbers (pixels|labels)
    for (i=0; i<num_data; i++) 
    {
        if (read(fd, data_char[i], arr_n * sizeof(unsigned char))== -1)
        {
           printf("pipo");
            exit(-1); 
        }   
    }

    close(fd);
}


void image_char2double(int num_data,int num_data_letter, unsigned char data_image_char[][SIZE],unsigned char data_image_char_letter[][SIZE], double data_image[][SIZE])
{
    int i, j;
    int a = 0;
    int b =0;

    for (i=0; i<num_data + num_data_letter; i++)
    {
        if (i% 5 == 0)
        {
            for (j=0; j<SIZE; j++)
            {
                data_image[i][j]  = (double)data_image_char[a][j] / 255.0;
            }
            a++;
        }
        else
        {
            for (j=0; j<SIZE; j++)
            {
                data_image[i][j]  = (double)data_image_char_letter[b][j] / 255.0;
            }
            b++;
        } 
    }      
}


void label_char2int(int num_data,int num_data_letter, unsigned char data_label_char[][1],unsigned char data_label_char_letter[][1], int data_label[])
{
    int i;
    int a = 0;
    int b =0;
    for (i=0; i<num_data + num_data_letter; i++)
        if (i% 5 == 0)
        {
            data_label[i]  = (int)data_label_char[a][0];
            a++;
        }
        else
        {
            data_label[i]  = (int)data_label_char_letter[b][0];
            b++;
        }
}


void load_mnist()
{
    read_mnist_char(TRAIN_IMAGE, NUM_TRAIN, LEN_INFO_IMAGE, SIZE, train_image_char, info_image);
    read_mnist_char(TRAIN_IMAGE_LETTRE, NUM_TRAIN_LETTRE, LEN_INFO_IMAGE, SIZE, train_image_char_letter, info_image);
    image_char2double(NUM_TRAIN , NUM_TRAIN_LETTRE, train_image_char,train_image_char_letter, train_image);

    read_mnist_char(TEST_IMAGE, NUM_TEST, LEN_INFO_IMAGE, SIZE, test_image_char, info_image);
    read_mnist_char(TEST_IMAGE_LETTRE, NUM_TEST_LETTRE, LEN_INFO_IMAGE, SIZE, test_image_char_letter, info_image);
    image_char2double(NUM_TEST , NUM_TEST_LETTRE, test_image_char,test_image_char_letter, test_image);
    
    read_mnist_char(TRAIN_LABEL, NUM_TRAIN, LEN_INFO_LABEL, 1, train_label_char, info_label);
    read_mnist_char(TRAIN_LABEL_LETTRE, NUM_TRAIN_LETTRE, LEN_INFO_LABEL, 1, train_label_char_letter, info_label);
    label_char2int(NUM_TRAIN , NUM_TRAIN_LETTRE, train_label_char,train_label_char_letter, train_label);
    
    read_mnist_char(TEST_LABEL, NUM_TEST, LEN_INFO_LABEL, 1, test_label_char, info_label);
    read_mnist_char(TEST_LABEL_LETTRE, NUM_TEST_LETTRE, LEN_INFO_LABEL, 1, test_label_char_letter, info_label);
    label_char2int(NUM_TEST , NUM_TEST_LETTRE, test_label_char,test_label_char_letter, test_label);
}


void print_mnist_pixel(double data_image[][SIZE], int num_data)
{
    int i, j;
    for (i=0; i<num_data; i++) {
        printf("image %d/%d\n", i+1, num_data);
        for (j=0; j<SIZE; j++) {
            printf("%1.1f ", data_image[i][j]);
            if ((j+1) % 28 == 0) putchar('\n');
        }
        putchar('\n');
    }
}


/*void print_mnist_label(int data_label[], int num_data)
{
    int i;
    if (num_data == NUM_TRAIN)
        for (i=0; i<num_data; i++)
            printf("train_label[%d]: %d\n", i, train_label[i]);
    else
        for (i=0; i<num_data; i++)
            printf("test_label[%d]: %d\n", i, test_label[i]);
}*/


// name: path for saving image (ex: "./images/sample.pgm")
/*void save_image(int n, char name[])
{
    char file_name[MAX_FILENAME];
    FILE *fp;
    int x, y;

    if (name[0] == '\0') {
        printf("output file name (*.pgm) : ");
        scanf("%s", file_name);
    } else strcpy(file_name, name);

    if ( (fp=fopen(file_name, "wb"))==NULL ) {
        printf("could not open file\n");
        exit(1);
    }

    fputs("P5\n", fp);
    fputs("# Created by Image Processing\n", fp);
    fprintf(fp, "%d %d\n", width[n], height[n]);
    fprintf(fp, "%d\n", MAX_BRIGHTNESS);
    for (y=0; y<height[n]; y++)
    {
        for (x=0; x<width[n]; x++)
        {
            fputc(image[n][x][y], fp);
        }
        fclose(fp);
        printf("Image was saved successfully\n");
    }
}


// save mnist image (call for each image)
// store train_image[][] into image[][][]
void save_mnist_pgm(double data_image[][SIZE], int index)
{
    int n = 0; // id for image (set to 0)
    int x, y;

    width[n] = 28;
    height[n] = 28;

    for (y=0; y<height[n]; y++) {
        for (x=0; x<width[n]; x++) {
            image[n][x][y] = data_image[index][y * width[n] + x] * 255.0;
        }
    }

    save_image(n, "");
}*/
