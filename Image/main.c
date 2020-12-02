#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "basics/sdl_basics.h"
#include "filter.h"
#include "segmentation.h"
#include "extraction.h"
#include "matrix_resize.h"
#include "skew.h"



int main()
{
    SDL_Surface* image;

    image = load_image("test_image/test.jpg");
    
    init_sdl();


    
    float gaussian_kernel[] = {0.0625, 0.125, 0.0625,
                               0.125, 0.25, 0.125,
			       0.0625, 0.125, 0.0625};
    
    applying_filter(image, gaussian_kernel);
    
    unsigned int otsu_value = Otsu_Method(image);
    binarization(image, otsu_value);


    double angle = find_angle(image);
    
    SDL_Surface* image_rotate = rotozoomSurface(image, angle, 1.0, 0);
    replace_new_pixels(image_rotate);

    horizontal_histogram(image_rotate);


    int nb_lines = number_of_lines(image_rotate);
    int char_per_line = 0;
    
    for(int i = 1; i <= nb_lines; i++)
    {
	SDL_Surface* line = cut_image(image_rotate,i);
	
	vertical_histogram(line);

	//SDL_SaveBMP(line,"lines/line.bmp");
   
	char_per_line = count_char(line);
	printf("In line %i there are %i characters\n",i,char_per_line);
	for(int j = 0; j <= char_per_line; j++)
	{
	    extraction(line, j);
	}
	  
	SDL_FreeSurface(line);
    }

    

    SDL_FreeSurface(image);

    SDL_FreeSurface(image_rotate);
    

    return 0;
}
