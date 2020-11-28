#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include "filter.h"
#include "segmentation.h"
#include <stdio.h>


int main()
{
    SDL_Surface* image;
    SDL_Surface* screen_surface;

    
    init_sdl();
    
    image = load_image("test_image/test6.bmp");
    
    screen_surface = display_image(image);


    
    wait_for_keypressed();
    
    grayscale(image);

    float gaussian_kernel[] = {0.0625, 0.125, 0.0625,
                               0.125, 0.25, 0.125,
			       0.0625, 0.125, 0.0625};
  
    applying_filter(image, gaussian_kernel);
    
    unsigned int otsu_value = Otsu_Method(image);
    binarization(image, otsu_value);

    update_surface(screen_surface, image);

    
    
    wait_for_keypressed();

    horizontal_segmentation(image);
    
    update_surface(screen_surface, image);

    
    
    wait_for_keypressed();

    vertical_segmentation(image);

    update_surface(screen_surface, image);
    



    wait_for_keypressed();

    extraction(image);

    update_surface(screen_surface, image);

    

    wait_for_keypressed();

    SDL_FreeSurface(image);

    SDL_FreeSurface(screen_surface);


    return 0;
}
