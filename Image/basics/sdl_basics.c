#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

/*
 * Initialize the SDL
 */


void init_sdl()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

/*
 * Load an image from a file
 */

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

/*
 * Display the image
 */

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }
    
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    return screen;
}


/*
 * Wait for a key to be pressed
 */

void wait_for_keypressed()
{
    SDL_Event event;
    
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

/*
 * Free the allocated memory used for loading and displaying
 */

void SDL_FreeSurface(SDL_Surface *surface);
