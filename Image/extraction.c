#include <stdio.h>
#include "segmentation.h"
#include "extraction.h"


void get_length(SDL_Surface* image,int row,int col, int width, int height, int *x, int *y)
{

  int current_pos_row = row;
  int current_pos_col = col;
  
  //Get the width of the char
  while(current_pos_col < width)
    {
      Uint32 pixel = get_pixel(image,current_pos_row,current_pos_col);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      if (r == 127)//if the current pixel is red, it means that we are at the end of the char area
	{
	  *x = current_pos_col - col;//Give to *x the value of the width
	  current_pos_col = width; 
	}
      current_pos_col++;
    }
  
  //Get the height of my char
  while(current_pos_row < height)
    {
      Uint32 pixel = get_pixel(image,current_pos_row,current_pos_col);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      if (r == 127)//if the current pixel is red, it means that we are at the end of the char area
	{
	  *y = current_pos_row - row;//Give to *y the value of the height
	  current_pos_row = height; 
	}
      current_pos_row++;
    }
}


void init_matrix
{
  m = malloc(sizeof(int)* x * y);
}

void fill_matrix(SDL_Surface* image, int start_row, int start_column)
{
   int width = image -> w;
   int height = image -> h;
   int row = start_row;
   int column = start_column;
   
   while(row < *x)//Start in the top left of the char area and stop at the bottom left
     {
       while(column < *y)//Start in the top left of the char area and stop at the top right
	 {
	   Uint32 pixel = get_pixel(image, row, column);
	   Uint8 r, g, b;
	   SDL_GetRGB(pixel, image->format, &r, &g, &b);
	   if(r == 0)
	      *m[row * (*y) + column] = 1;
	   else
	     *m[row * (*y) + column] = 0;
	 }
     }
  
}


void extraction(SDL_Surface* image)
{
  int width = image -> w;
  int height = image -> h;

  int row = 0;
  int column = 0;
  int *m;//Probably need to create pointers for each call 
  int *x;//So maybe it need to be move in the while
  int *y;
  
    
    while (row < height)
    {
	while(column < width)
	  {
	    Uint32 pixel = get_pixel(image, row, column);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);

	    if(r != 127)//Go through the image and when we find a character 
	      {
		get_length(image,row, column, width, height, p, x, y);//First we get the length
		init_matrix();//Then we create a matrix of with good 
		fill_matrix();//And then we fill the matrix with 0 - 1
		column+= x; 
	      }
	    column++;
	  }
	row += y + 1;//On saute à la prochaine ligne rouge
	column = 0;//Et on se replace au bord gauche de la page
}
      
      
      
	  
