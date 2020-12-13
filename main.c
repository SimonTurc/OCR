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

#include "text.h"

#define CONTRAST 150

struct text* get_text(char* filename)
{
    SDL_Surface *image;
    SDL_Surface *image_median;
    SDL_Surface *image_gaussian;
    
    SDL_Surface *image_rotate;
    SDL_Surface *line;
    
    double angle = 0;
    int nb_lines = 0;
    int char_per_line = 0;
    //int nb_space = 0;
    unsigned int otsu_value = 0;

    struct text *result = newtext();
    image = load_image(filename);
    image_gaussian = load_image(filename);
    image_median = load_image(filename);

    contrast_adjustment(image, CONTRAST);
    contrast_adjustment(image_gaussian, CONTRAST);
    contrast_adjustment(image_median, CONTRAST);
    
    init_sdl();

    grayscale(image_median);
    grayscale(image_gaussian);
    grayscale(image);
    
    median_filter(image_median);

    applying_filter(image_gaussian);

    otsu_value = Otsu_Method(image);
    binarization(image, otsu_value);
    binarization(image_median, otsu_value);
    binarization(image_gaussian, otsu_value);
    
    compute_filters(image, image_gaussian, image_median);


    if (var_histo(image_median) > var_histo(image))
    {
	
	angle = find_angle(image_median);

	image_rotate = rotozoomSurface(image_median, angle, 1.0, 0);
	replace_new_pixels(image_rotate);

    }
    else
    {
	
	angle = find_angle(image);

	image_rotate = rotozoomSurface(image, angle, 1.0, 0);
	replace_new_pixels(image_rotate);

    }

    
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
	//nb_space = count_space(line);
        // printf("In line %i there are %i characters\n",i,char_per_line);
        // printf(" char per line :%i\n", char_per_line);
        for (int j = 1; j <= char_per_line; j++)
        {
	  // printf("%c", extraction(line, j));
	  text_push(result, extraction(line,j));
	  // printf(",\n");
	  // printf("%i,", number);
	  // number++;
        }
	text_push(result,'\n');
        //printf("\n");
        SDL_FreeSurface(line);
    }
    // printf("};");

    SDL_FreeSurface(image);

    SDL_FreeSurface(image_gaussian);

    SDL_FreeSurface(image_median);

    SDL_FreeSurface(image_rotate);

    return result;
}


int main(){
  char* filename = "test_image/18.png";
  struct text *result = get_text(filename);
  if(result->size > 0)
      {
	for(size_t k = 0; k < result->size; k++)
	  {
	    printf("%c", result->data[k]);
	  }
      }
  freetext(result);
}
