#include "basics/pixel_operations.h"
#include "matrix_resize.h"
#include "prediction.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int count_char(SDL_Surface *image)
{
    int width = image->w;
    int isWhite = 0;
    int col = 0;
    int nb_char = 0;

    while (col < width)
    {
        Uint32 pixel = get_pixel(image, col, 0);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if ((g == 255 || g == 1) && isWhite == 0) // Si le pixel est noir ou blanc
        {
            isWhite = 1; // On est dans un char donc isWhite passe a 1
            nb_char += 1;
        }

        if (isWhite == 1)
        {
            if (g == 127)
            {
                isWhite = 0;
            }
        }
        col++;
    }
    return nb_char;
}

int matrix_col_extract(SDL_Surface *image, int char_nb)
{
    int width = image->w;
    int col_to_return = 0;
    int isWhite = 0;
    int col = 0;
    int current_char = 0;

    while (current_char < char_nb && col < width)
    {
        Uint32 pixel = get_pixel(image, col, 0);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if ((g == 255 || g == 1) && isWhite == 0) // Si le pixel est noir ou blanc
        {
            isWhite = 1;       // On est dans un char donc isWhite passe a 1
            current_char += 1; // Le n-ieme char de la ligne
            col_to_return = col;
        }

        if (isWhite == 1)
        {
            if (g == 127)
            {
                isWhite = 0;
            }
        }
        col++;
    }
    return col_to_return;
}

int get_length_col(SDL_Surface *image, int width, int col)
{
    int x = 0;
    int start_col = col;

    // Get the width of the char
    while (col < width)
    {
        Uint32 pixel = get_pixel(image, col, 0);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if (g == 127) // if the current pixel is red, it means that we are at the end of the char area
        {

            x = col - start_col; // Give to x the value of the width
            // col = width;
            return x;
        }
        col++;
    }
    return x;
}

double *fill_matrix(SDL_Surface *image, int start_col, int end_col, double *m)
{
    int height = image->h;

    for (int i = 0; i < height; i++)
    {
        for (int j = start_col; j < end_col; j++)
        {
            Uint32 pixel = get_pixel(image, j, i);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 1)
            {
                m[(j - start_col) + i * (end_col - start_col)] = 1.0;
            }
        }
    }
    return m;
}

char extraction(SDL_Surface *image, int char_nb)
{
    char letter;
    int width = image->w;
    int height = image->h;
    int start = matrix_col_extract(image, char_nb);
    int x = get_length_col(image, width, start); // First we get the length
    size_t output_size = 28;

    double *m = (double *)malloc(sizeof(double) * x * height);
    double *m1 = (double *)malloc(sizeof(double) * output_size * output_size);
    memset(m, 0, x * height * sizeof(double));
    memset(m1, 0, output_size * output_size * sizeof(double));
    m = fill_matrix(image, start, start + x, m);
    matrix_resize(m, height, x, m1); // And then we fill the matrix with 0 - 1
    // printf("{");
    for (size_t i = 0; i < output_size; i++)
    {
        for (size_t j = 0; j < output_size; j++)
        {
            /*if (i == output_size - 1 && i == j)
            {
                printf("%1.f ", m1[i * (output_size) + j]);
            }
            else
            {
                printf("%1.f, ", m1[i * (output_size) + j]);
            }*/

            /*printf("%ld   %ld", i, j);
            if ((j + 1) % x == 0)
                putchar('\n');*/
        }
        if (i < output_size - 1)
        {
            // printf("\n");
        }
    }
    // printf("},\n");
    letter = predict(m1);

    // letter = 'a';

    free(m);
    free(m1);

    return letter;
}
