#include <stdio.h>
#include <err.h>
#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include "skew.h"
#include "SDL/SDL_rotozoom.h"

/*
 * angle_histogram : remplit un histogram horizontal d'une image
 */
void angle_histogram(SDL_Surface* image_rotate, int histo[])
{
    int width = image_rotate->w;
    int height = image_rotate->h;
   

    for(int k = 0; k < height; k++)
	histo[k] = 0;

    for(int j = 0; j < height; j++)
    {
	for (int i = 0; i < width; i++)
	{
	    Uint32 pixel = get_pixel(image_rotate, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image_rotate->format, &r, &g, &b);
	    
	    if (r < 127)
	    {
		histo[j]++;
	    }
	}
    }
}


/*
 * find_angle : retourne l'angle le plus propice pour corriger la rotation
 */
double find_angle(SDL_Surface* image)
{
    double angle = 0;
    double variance = 0;
    double max = 0;

    for(double r = -60; r <= 60; r += 2)
    {
	SDL_Surface* rotation = rotozoomSurface(image, r, 1.0, 0);
	replace_new_pixels(rotation);
	int histo[rotation->h];
	angle_histogram(rotation,histo);
	variance = variance_histogram(histo, rotation->h);
	
	if (max < variance)
	{
	    max = variance;
	    angle = r;

	}
	SDL_FreeSurface(rotation);
    }
    return angle;
}


/*
 * Variance_histogram retourne la variance d'un histogram
 */
double variance_histogram(int histo[], int height)
{
    double M = 0;
    double Var = 0;
    double s1 = 0;
    double s2 = 0;
    
    for(int i = 0; i < height; i++)
    {
	s1 += histo[i];
	s2 += histo[i] * histo[i];
    }

    M = s1/height;
    
    Var = s2/height - M * M;

    return Var;
}

void replace_new_pixels(SDL_Surface* image)
{
    int width = image->w;
    int height = image->h;

    for(int j = 0; j < height; j++)
    {
	for(int i = 0; i < width; i++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    Uint32 average = (r + g + b)/3;

	    if (average == 0)
	    {
		pixel = SDL_MapRGB(image->format, 255, 255, 255);
		put_pixel(image, i, j, pixel);
	    }
	}
    }
}
