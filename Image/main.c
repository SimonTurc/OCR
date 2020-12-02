#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "basics/sdl_basics.h"
#include "filter.h"
#include "segmentation.h"
#include "extraction.h"



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


    
    horizontal_histogram(image);

    int nb_lines = number_of_lines(image);

    for(int i = 1; i <= nb_lines; i++)
    {
	SDL_Surface* line = cut_image(image,i);
	
	vertical_histogram(line);

	SDL_SaveBMP(line,"lines/line.bmp");
	if(i == nb_lines)
	  {
	    for(int j = 0; j < 15; j++)
	      {
		extraction(line, j);
	      }
	  }
	SDL_FreeSurface(line);
    }

    

    SDL_FreeSurface(image);
    

    return 0;
}
