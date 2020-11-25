#include "basics/pixel_operations.h"
#include "basics/sdl_basics.h"
#include "segmentation.h"
#include <stdio.h>

/*
 * Supprimer le border horizontal de l'image
 */

void horizontal_segmentation(SDL_Surface* image){
    int width = image -> w;
    int height = image -> h;

    int row = 0;
    int column = 0;
    
    
    while (row < height)
    {
	while(column < width)
	{
	    Uint32 pixel = get_pixel(image, column, row);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);

	    if (r == 0) 
	    {
		column = 0; //Go to the next line
		row++;
	    }

	    if (column == width - 1)
	    {
		for (int k = 0; k < width; k++)
		{
		    pixel = SDL_MapRGB(image->format , 127 , 0 , 0);
		    put_pixel(image, k, row, pixel);	       
		}
	    }
	    column++;
	    
	}
	column = 0;
	row++;
    }
    
}

void vertical_segmentation(SDL_Surface* image){
    int width = image -> w;
    int height = image -> h;

    int row = 0;
    int column = 0;
    int isRed = 0;
    int FirstBlank = 0;
    int isBlack = 0;
    
    
    while (column < width)
    {

	isBlack = 0;
	isRed = 0;

	while (row < height)
	{
            
	    Uint32 pixel = get_pixel(image, column, row);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
            
	    if ((r == 255 || r == 0) && isRed != 1)
	    {
                if(r == 0)
                {
                    isBlack = 1;
                    isRed = 1;
                    FirstBlank = row;
                }
                else
                {
                    isRed = 1;
                    FirstBlank = row;
                    isBlack = 0;
                }
	    }
             
	    if (r != 127 && isRed == 1)
	    {

		if (r == 0) 
		{
		     //Go to the next 
		    isBlack = 1;
		}
	    }

	    if (r == 127 && isRed == 1)
	    {
	       
		isRed = 0;
		if (isBlack != 1)
		{
		    for (int k = FirstBlank; k < row; k++)
		    {
			pixel = SDL_MapRGB(image->format , 127 , 0 , 0);
			put_pixel(image, column, k, pixel);	       
		    }
		}
		isBlack = 0;

	    }
            
            
	    row++;
	   
	}
	row = 0;
	column++;
    }
    
}


