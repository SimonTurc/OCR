#include <err.h>
#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include <math.h>

/*
 * Classic Thresholding (Otsu Methode)
 */

unsigned int Otsu_Method(SDL_Surface* image){
    
    int width = image -> w;
    int height = image -> h;
    
    int *histo = calloc(256, sizeof(int));
    if (histo == NULL)
	errx(1, "Not enough memory!");
    
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

    //for(size_t k = 0; k < 256; k++)
    //histo[k] = 0;

    
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
	s2 += histo[255 - v];

	w1 = s1 / number_of_pixels;
	w2 = s2 / number_of_pixels;
	q1 += v * histo[v];
	q2 += (255 - v) * histo[255 - v];

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

    free(histo);
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


/*
 * Fonction qui applique le gaussian filter
 */

void gaussian_filter(SDL_Surface* image)
{
    int width = image -> w;
    int height = image -> h;
    int pixel_x = 0;
    int pixel_y = 0;
    
    /* 5x5 Filterfloat gaussian_kernel[] = {0.00390625, 0.015625,  0.0234375,  0.015625, 0.00390625 ,
				 0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
				 0.0234375, 0.09375, 0.140625, 0.09375, 0.0234375,
				 0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
				 0.00390625 , 0.015625,  0.0234375,  0.015625, 0.00390625 };*/


    float gaussian_kernel[] = {0.0625, 0.125, 0.0625,
                               0.125, 0.25, 0.125,
			       0.0625, 0.125, 0.0625};
	
    int offset = 3 / 2;
    
    for(int i = offset; i < width - offset ; i++)
    {
	for(int j = offset; j < height - offset; j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    unsigned int  acc[3] = {0 , 0 , 0};
	    for(int x = 0; x < 3; x++)
	    {
		for(int y = 0; y < 3; y++)
		{
		    pixel_x = i + x - offset;
		    pixel_y = j + y - offset;

		     Uint32 pixel2 = get_pixel(image, pixel_x, pixel_y);
		     Uint8 r, g, b;
		     SDL_GetRGB(pixel2, image->format, &r, &g, &b);

		     acc[0] += r * gaussian_kernel[x * 3 + y];
		     acc[1] += g * gaussian_kernel[x * 3 + y];
		     acc[2] += b * gaussian_kernel[x * 3 + y];
		}
	    }
	    pixel = SDL_MapRGB(image->format, acc[0], acc[1], acc[2]);
	    put_pixel(image, i, j, pixel);
	    
	}
    }
}
