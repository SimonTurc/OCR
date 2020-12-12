#include <stdio.h>

#include "SDL/SDL.h"

#include "SDL/SDL_image.h"

#include "SDL/SDL_rotozoom.h"

#include "Image/basics/sdl_basics.h"

#include "Image/filter.h"

#include "Image/segmentation.h"

#include "Image/extraction.h"

#include "Image/matrix_resize.h"

#include "Image/skew.h"

int main()
{
    SDL_Surface *image;
    SDL_Surface *image_rotate;
    SDL_Surface *line;
    double angle = 0;
    int nb_lines = 0;
    int char_per_line = 0;
    unsigned int otsu_value = 0;

    float gaussian_kernel[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};

    image = load_image("test_image/cp11.png");

    init_sdl();

    applying_filter(image, gaussian_kernel);

    otsu_value = Otsu_Method(image);
    binarization(image, otsu_value);

    angle = find_angle(image);

    image_rotate = rotozoomSurface(image, angle, 1.0, 0);
    replace_new_pixels(image_rotate);

    horizontal_histogram(image_rotate);

    nb_lines = number_of_lines(image_rotate);
    char_per_line = 0;
    // int number = 0;
    // printf("double dataset_train[61][784] = { \n");
    // printf("double dataset_test[52] = {");
    for (int i = 1; i <= nb_lines; i++)
    {
        line = cut_image(image_rotate, i);

        vertical_histogram(line);

        char_per_line = count_char(line);
        // printf("In line %i there are %i characters\n",i,char_per_line);
        // printf(" char per line :%i\n", char_per_line);
        for (int j = 1; j <= char_per_line; j++)
        {
            // printf("%c", extraction(line, j));
            extraction(line, j);
            // printf(",\n");
            // printf("%i,", number);
            // number++;
        }
        printf("\n");
        SDL_FreeSurface(line);
    }
    // printf("};");

    SDL_FreeSurface(image);

    SDL_FreeSurface(image_rotate);

    return 0;
}
