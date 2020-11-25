#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include <math.h>

/*
 * Classic Thresholding (Otsu Methode)
 */

unsigned int Otsu_Method(SDL_Surface* image){
    
    int width = image -> w;
    int height = image -> h;
    
    int histo[256];
    int number_of_pixels = 0;

    double q1 = 0;
    double q2 = 0;
    double m1 = 0;
    double m2 = 0;
    double w1 = 0;
    double w2 = 0;
    double s1 = 0;
    double s2 = 0;
    double max = 0;
    double inter = 0;
    double tmp = 0;
    unsigned int value = 0;

    for(size_t k = 0; k < 256; k++)
	histo[k] = 0;

    
    for (int i = 0; i < width; i++)
    {
	for(int j = 0; j < height; j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    Uint32 average = (r + g + b)/3;

	    histo[average]++;
	    number_of_pixels++;
	}
    }

    

    for (unsigned int v = 0; v < 256; v++)
    {
	s1 += histo[v];
	s2 += histo[256 - v];

	w1 = s1 / number_of_pixels;
	w2 = s2 / number_of_pixels;
	q1 += v * histo[v];
	q2 += (256 - v) * histo[256 - v];

	m1 = q1 / s1;
	m2 = q2 / s2;

	tmp = (m1 - m2) * (m1 - m2);
	inter = w1 * w2 * tmp;

	if (inter > max)
	{
	    value = v;
	    max = inter;
	}	    
	   
    }

    return value;
}

void binarization(SDL_Surface* image,unsigned int otsu_value){
    int width = image -> w;
    int height = image -> h;
    //unsigned int value = Otsu_Method(image);

    
    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height;j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    Uint32 average = (r + g + b)/3;

	    if (average <= otsu_value)
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

/*
 * Filtre de niveau de gris
 */

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



/*double gaussian(int x,int y, double sigma)
{
    return 1. / (2* M_PI * sigma * sigma) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}*/


/*
 * Fonction de construction d'un kernel(= matrice à appliquer sur l'image)
 */

void Buildkernel(size_t x , size_t y, double filter[x][y])
{
    //double sigma = 0.84089642;

    for(size_t i = 0; i < x; i++)
	for(size_t j = 0; j < y; j++)
	    filter[i][j] = 1;               //gaussian(x , y, sigma);
}


/*
 * Fonction qui applique cette matrice sur l'ensemble de l'image
 */

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
