#include <err.h>
#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include "filter.h"

/*
 * Classic Thresholding (Otsu Methode)
 */

unsigned int Otsu_Method(SDL_Surface* image){
    
    int width = image -> w;
    int height = image -> h;
    
    int *histo = calloc(256, sizeof(int));
    if (histo == NULL)
	errx(1, "Otsu_Method : not enough memory for the histogram.");
    
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
		pixel = SDL_MapRGB(image->format, 1, 1, 1);
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


/*Subfunction*/
void array_swap(unsigned int array[], size_t i, size_t j)
{
    size_t in_memory = array[i];
    array[i] = array[j];
    array[j] = in_memory;
}


/* Subfunction */
size_t __array_min_index(unsigned int array[], size_t i, size_t len)
{
    size_t min_index = i;
    for (; i < len; i++)
    {
	if (array[min_index] >  array [i])
	    min_index = i;
    }
    return min_index;
}


/* Selection sort */
void array_select_sort(unsigned int array[], size_t len)
{
    size_t min;
    
    for(size_t i = 0; i < len; i++)
    {
	min = __array_min_index(array,i,len);
	array_swap(array,i,min);
    }
}

/*
 * Fonction qui applique le
 */

void median_filter(SDL_Surface* image)
{

    int width = image -> w;
    int height = image -> h;
    int pixel_x = 0;
    int pixel_y = 0;
	
    int offset = 5 / 2;

    for(int j = offset; j < height - offset; j++)
    {
	for(int i = offset; i < width - offset ; i++)
	{
	    int k = 0;
	    Uint32 pixel = get_pixel(image, i, j);
	    
	    unsigned int  acc[25];
	    
	    for(int z = 0; z < 25; z++)
	    {
		acc[z] = 0;
	    }
	   


	    for(int x = 0; x < 5; x++)
	    {
		for(int y = 0; y < 5; y++)
		{
		    pixel_x = i + x - offset;
		    pixel_y = j + y - offset;

		     Uint32 pixel2 = get_pixel(image, pixel_x, pixel_y);
		     Uint8 r, g, b;
		     SDL_GetRGB(pixel2, image->format, &r, &g, &b);

		    
		     acc[k] += r;
		     k++;
		     
		}
	    }

	    
	    unsigned int around_pixels[] = {acc[0], acc[1], acc[2], acc[3], acc[4], acc[5], acc[6], acc[7], acc[8], acc[9], acc[10], acc[11], acc[12],  acc[14], acc[15], acc[16], acc[17], acc[18], acc[19], acc[20], acc[21], acc[22], acc[23], acc[24]};
	    unsigned int Ileft = acc[12];
	    unsigned int Iij = acc[13];
	    array_select_sort(around_pixels, 24);
	    unsigned int Imin = around_pixels[0];
	    unsigned int Imax = around_pixels[23];
	    unsigned int Imed = (around_pixels[12] + around_pixels[13])/2;

	    //Stage A
	    if(Imin < Iij && Iij < Imax)
	    {
		continue;
	    }
	    else
	    {
		//Stage B Iij should be an impulse noise
		if ((Imin < Imed && Imed < Imax) || (0 < Imed && Imed < 255 ))
		{
		    pixel = SDL_MapRGB(image->format, Imed, Imed, Imed);
		    put_pixel(image, i, j, pixel);
		}
		else
		{
		    //Stage C
		    pixel = SDL_MapRGB(image->format, Ileft, Ileft, Ileft);
		    put_pixel(image, i, j, pixel);
		}
		
	    }
	    
	}
    }

    adjust_border(image);
}

void adjust_border(SDL_Surface* image)
{
    int width = image -> w;
    int height = image -> h;
    Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);

    for(int i = 0; i < width; i++)
    {
	put_pixel(image, i, 0, pixel);
	put_pixel(image, i, height - 1, pixel);
    }

    for(int j = 0; j < height; j++)
    {
	put_pixel(image, 0, j, pixel);
	put_pixel(image, width - 1, j, pixel);
    }
    
}

void compute_filters(SDL_Surface* image,SDL_Surface* image_gaussian,SDL_Surface* image_median)
{
    int width = image -> w;
    int height = image -> h;

    for (int i = 0; i < width; i++)
    {
	for (int j = 0; j < height;j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint32 pixel_gaussian = get_pixel(image_gaussian, i, j);
	    Uint32 pixel_median = get_pixel(image_median, i,j);
	    
	    Uint8 r, g, b;
	    Uint8 r_g, g_g, b_g;
	    Uint8 r_m, g_m, b_m;
	    
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    SDL_GetRGB(pixel_gaussian, image->format, &r_g, &g_g, &b_g);
	    SDL_GetRGB(pixel_median, image->format, &r_m, &g_m, &b_m);

	    if(r < 127 && r_m > 127 && r_g < 127)
		put_pixel(image_median, i, j, pixel);
	    
	}
    }
}

/*
 * Fonction qui applique un filtre gaussien
 */

void applying_filter(SDL_Surface* image)
{
    float kernel[25] = {0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625,
				 0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
				 0.0234375, 0.09375, 0.140625, 0.09375, 0.0234375,
				 0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
				 0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625};
    
    int width = image -> w;
    int height = image -> h;
    int pixel_x = 0;
    int pixel_y = 0;
	
    int offset = 5 / 2;
    
    for(int i = offset; i < width - offset ; i++)
    {
	for(int j = offset; j < height - offset; j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    unsigned int  acc[3] = {0 , 0 , 0};
	    for(int x = 0; x < 5; x++)
	    {
		for(int y = 0; y < 5; y++)
		{
		    pixel_x = i + x - offset;
		    pixel_y = j + y - offset;

		     Uint32 pixel2 = get_pixel(image, pixel_x, pixel_y);
		     Uint8 r, g, b;
		     SDL_GetRGB(pixel2, image->format, &r, &g, &b);

		     acc[0] += r * kernel[x * 5 + y];
		     acc[1] += g * kernel[x * 5 + y];
		     acc[2] += b * kernel[x * 5 + y];
		}
	    }
	    pixel = SDL_MapRGB(image->format, acc[0], acc[1], acc[2]);
	    put_pixel(image, i, j, pixel);
	    
	}
    }

    adjust_border(image);
}

void thickness(SDL_Surface* image)
{
    //float kernel[9] = {0.0625, 0.125, 0.0625,0.125, 0.25, 0.125,0.0625, 0.125, 0.0625};

    float kernel[9] = {0.05,  0.15, 0.05,
		       0.15 , 0.20, 0.15,
		       0.05 , 0.15, 0.05};
    
    int width = image -> w;
    int height = image -> h;
    int pixel_x = 0;
    int pixel_y = 0;
	
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

		     acc[0] += r * kernel[x * 3 + y];
		     acc[1] += g * kernel[x * 3 + y];
		     acc[2] += b * kernel[x * 3 + y];
		}
	    }

	    if (acc[0] < 127 && acc[1] < 127 && acc[2])
	    {
		pixel = SDL_MapRGB(image->format, acc[0], acc[1], acc[2]);
		put_pixel(image, i, j, pixel);
	    }
	    
	}
    }

    adjust_border(image);
}


double var_histo(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;
    int histo[height];
    
    for(int j = 0; j < height; j++)
    {
	for(int i = 0; i < width; i++)
	{
	    histo[j] += i;
	}
    }
    
    double M = 0;
    double Var = 0;
    double s1 = 0;
    double s2 = 0;
    
    for(int k = 0; k < height; k++)
    {
	s1 += histo[k];
	s2 += histo[k] * histo[k];
    }

    M = s1/height;
    
    Var = s2/height - M * M;

    return Var;
}

void contrast_adjustment(SDL_Surface* image, float contrast)
{
    int width = image -> w;
    int height = image -> h;
    float factor = (259. * (contrast + 255.)) / (255. * (259. - contrast));
    
    for(int i = 0; i < width; i++)
    {
	for(int j = 0; j < height; j++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    
	    int new_r = truncate1(factor * (r - 128) + 128);
	    int new_g = truncate1(factor * (g - 128) + 128);
	    int new_b = truncate1(factor * (b - 128) + 128);
	    
	    pixel = SDL_MapRGB(image->format, new_r, new_g, new_b);
	    put_pixel(image, i, j, pixel);
	    
	}
    }
						 
}

int truncate1(float value)
{
    if (value >= 255.)
    {
	return 255;
    }
    else if (value <= 0.)
    {
	return 0;
    }
    else
    {
	return value;
    }
}
