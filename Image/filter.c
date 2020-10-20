#include "pixel_operations.h"
#include "sdl_basics.h"
#include "filter.h"

void binarization(SDL_Surface* image){
    int width = image -> w;
    int height = image -> h;
    
    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height;j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    Uint32 average = (r + g + b)/3;

	    if (average <= 127.5)
	    {
		pixel = SDL_MapRGB(image->format, 0, 0, 0);
		put_pixel(image, i, j, pixel);
	    }
	    else
	    {
		pixel = SDL_MapRGB(image->format, 255, 255, 255);
		put_pixel(image, i, j, pixel);
	    }
	}
    }
}

void grayscale(SDL_Surface* image){
    int width = image -> w;
    int height = image -> h;
    
    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height;j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    Uint32 average = 0.3*r + 0.59*g + 0.11*b;
	    pixel = SDL_MapRGB(image->format, average, average, average);
	    put_pixel(image, i, j, pixel);
	    
	}
    }
}
