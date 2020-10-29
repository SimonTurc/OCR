#include "pixel_operations.h"
#include "sdl_basics.h"
#include "filter.h"
#include <math.h>

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

double gaussian(int x,int y, double sigma)
{
    return 1. / (2* M_PI * sigma * sigma) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

void Buildkernel(size_t x , size_t y, double filter[x][y])
{
    double sigma = 0.84089642;

    for(size_t i = 0; i < x; i++)
	for(size_t j = 0; j < y; j++)
	    filter[i][j] = gaussian(x , y, sigma);
}


void applyingfilter(SDL_Surface* image)
{
    int width = image -> w;
    int height = image -> h;

    Uint32 rSum = 0;
    Uint32 gSum = 0;
    Uint32 bSum = 0;

    double kernel[3][3];
    Buildkernel(3,3,kernel);
    
    for (int i = 1; i < width - 1; i++)
    {
	for (int j = 1; j < height - 1;j++)
	{
	    
	    for( int i_around = -1 ; i_around <= 1; i_around++)
	    {
		for( int j_around = -1; j_around <= 1; j_around++)
		{
		    Uint32 pixel = get_pixel(image, i + i_around, j + j_around);
		    Uint8 r, g, b;
		    SDL_GetRGB(pixel, image->format, &r, &g, &b);
		    
		    rSum += r * kernel[1 + i_around][1 + j_around];
		    gSum += g * kernel[1 + i_around][1 + j_around];
		    bSum += b * kernel[1 + i_around][1 + j_around];
		}
	    }

	    Uint32 new_pixel = get_pixel(image, i, j);
	    
	    rSum = rSum/9;
	    gSum = gSum/9;
	    bSum = bSum/9;
	    
	    new_pixel = SDL_MapRGB(image->format, rSum, gSum, bSum);
	    put_pixel(image, i, j, new_pixel);
	    
	}
    }
}
