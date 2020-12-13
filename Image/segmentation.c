#include <stdio.h>
#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include "segmentation.h"


/*
 *  Horizontal_histogram : projection horizontale par histogram (segmentation horizontale)
 */
void horizontal_histogram(SDL_Surface* image){
    int width = image -> w;
    int height = image -> h;

    int histo[height];

    for (int k = 0; k < height; k++)
	histo[k] = 0;


    // Création de l'histogramme des pixels noirs par rangée
    for(int j = 0; j < height; j++)
    {
	for (int i = 0; i < width; i++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    
	    if (r < 127)
	    {
		histo[j]++;
	    }
	}
    }

    //Trace des lignes horizontale en rouge
    for(int y = 0; y < height; y++)
    {
	if (histo[y] < 2)
	{
	    for(int x = 0; x < width; x++)
	    {
		 Uint32 new_pixel = get_pixel(image, x, y);
		 Uint8 r, g, b;
		 SDL_GetRGB(new_pixel, image->format, &r, &g, &b);
		 
		 new_pixel = SDL_MapRGB(image->format , 127 , 0 , 0);
		 put_pixel(image, x, y, new_pixel);
	    }
	}
     }
}





/*
 * Cut_lines : découpe une ligne de l'image
 */
SDL_Surface* cut_lines(SDL_Surface* image, int first, int last)
{
    int width = image -> w;

    int y = 0;

    //Création d'un nouvelle image pour contenir une ligne
    SDL_Surface* image_line = SDL_CreateRGBSurface(0, width, last-first, 32, 0, 0, 0, 0);

    for(int j = first; j < last; j++)
    {
	for(int i = 0; i < width; i++)
	{
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);

	    
	    Uint32 pixel_copy = SDL_MapRGB(image_line->format , r , g , b);
	    put_pixel( image_line, i, y, pixel_copy);
	    
      
	}
	y++;
    }
    return image_line;
}



/*
 * Cut_ image : découpe l'image en fonctions lignes rouges horizontale sous forme de ligne
 */
SDL_Surface* cut_image(SDL_Surface* image, int nb_image)
{    
    int height = image -> h;
    
    int first = 0;
    int last = 0;

    int number = 1;
    int isLine = 0;

    SDL_Surface* line = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
    
    for(int j = 0; j < height; j++)
    {
	 Uint32 pixel = get_pixel(image, 0, j);
	 Uint8 r, g, b;
	 SDL_GetRGB(pixel, image->format, &r, &g, &b);

	 if (r != 127 && (isLine == 0))
	 {
	     first = j;
	     isLine = 1;
	 }

	 if (r == 127 && (isLine == 1))
	 {
	     last = j-1;
	     isLine = 0;

	     if(number == nb_image)
	     {
		 free(line);
		 return line = cut_lines(image,first,last);	 
	     }
	     number++;
	  
	 }
    }

    return line;
}


/*
 * Number_of_line: Compte le nombre de ligne
 */
int number_of_lines(SDL_Surface* image)
{
    int height = image -> h;
    
    int isLine = 0;
    int number = 0;
    
    for(int j = 0; j < height; j++)
    {
	 Uint32 pixel = get_pixel(image, 0, j);
	 Uint8 r, g, b;
	 SDL_GetRGB(pixel, image->format, &r, &g, &b);

	 if (r != 127 && (isLine == 0))
	 {
	     isLine = 1;
	 }

	 if (r == 127 && (isLine == 1))
	 {
	     isLine = 0;
	     number++;
	 }
    }

    return number;
}




/*
 * Vertical_histogram : projection horizontale par histogram (segmentation horizontale)
 */
void vertical_histogram(SDL_Surface* ligne)
{
    int width = ligne -> w;
    int height = ligne -> h;

    int histo[width];

    for(int k = 0; k < width; k++)
	histo[k] = 0;
	

    //Construction de l'histogramme des pixels noirs par colones sur une ligne 
    for(int i = 0; i < width; i++)
    {
	for (int j = 0; j < height; j++)
	{
	    Uint32 pixel = get_pixel(ligne, i, j);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, ligne->format, &r, &g, &b);
	    
	    if (r < 127)
		histo[i]++;
	    
	}
    }

    // Trace la ligne de pixel verticale en vert
    for(int x = 0; x < width; x++)
    {
	if (histo[x] < height * 0.01)
	{
	    for(int y = 0; y < height; y++)
	    {
		 Uint32 new_pixel = get_pixel(ligne, x, y);
		 Uint8 r, g, b;
		 SDL_GetRGB(new_pixel, ligne->format, &r, &g, &b);
		 
		 new_pixel = SDL_MapRGB(ligne->format , 0 , 127, 0);
		 put_pixel(ligne, x, y, new_pixel);
	    }
	}
     }
}
