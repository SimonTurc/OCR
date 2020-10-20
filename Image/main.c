#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "sdl_basics.h"
#include "filter.h"
#include "segmentation.h"


int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    
    
    init_sdl();
    
    image_surface = load_image("test.jpg");

    screen_surface = display_image(image_surface);
    


    wait_for_keypressed();

    grayscale(image_surface);

    binarization(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    horizontal_segmentation(image_surface);
    
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed();

    vertical_segmentation(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    SDL_FreeSurface(image_surface);

    SDL_FreeSurface(screen_surface);


    return 0;
}
