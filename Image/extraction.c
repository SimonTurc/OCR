#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "basics/pixel_operations.h"


int get_length_col(SDL_Surface* image,int row,int col,int width, int height)
{

  int current_pos_row = row;
  int current_pos_col = col;
  int x = 0;
  //printf("%i\n", height);
  //printf("%i\n", width);

  
  //Get the width of the char
  while(current_pos_col < width)
    {
      Uint32 pixel = get_pixel(image,current_pos_col,current_pos_row);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      if (r == 127)//if the current pixel is red, it means that we are at the end of the char area
	{
	  pixel = SDL_MapRGB(image->format , 0 , 200 , 0);
	  put_pixel(image, current_pos_col-1, current_pos_row, pixel);
	  x = current_pos_col - col-1;//Give to *x the value of the width
	  printf("Col: Col %i, Col_pos: %i, Diff %i\n", col, current_pos_col, current_pos_col-col);
	  current_pos_col = height;// = width a modif
	  return x;
	}
      
      current_pos_col++;
    }
    printf("Col: %i, %i, %i\n", col, current_pos_col, current_pos_col-col);
    return x;
}
  
  //Get the height of my char

int get_length_row(SDL_Surface* image,int row,int col,int width, int height)
{
  int current_pos_row = row;
  //int current_pos_col = col;
  
  int y = 0;
  printf("%i\n", height);
  printf("%i\n", width);
  
  while(current_pos_row < height)
    {
      Uint32 pixel = get_pixel(image,col,current_pos_row);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      if (r == 127)//if the current pixel is red, it means that we are at the end of the char area
	{
	  pixel = SDL_MapRGB(image->format , 0 , 0 , 200);
	  put_pixel(image, col, current_pos_row-1, pixel);
	  y = current_pos_row - row;//Give to *y the value of the height
	  printf("Row: Row %i, Row_pos: %i, Diff %i\n", row, current_pos_row, current_pos_row-row);
	  current_pos_row = height;
	  return y;
	}

      current_pos_row++;
    }
     //printf("Row: %i, %i, %i\n", row, current_pos_row, current_pos_row-row);
     return y;
}



/*void init_matrix()
{
  m = malloc(sizeof(int)* (*x) * (*y));
}*/

void fill_matrix(SDL_Surface* image, int start_row, int start_column, int *x, int *y)
{
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
	     {
	       //*m[row * (*y) + column] = 1;
	       pixel = SDL_MapRGB(image->format , 0 , 0 , 200);
	       put_pixel(image, row, column, pixel);
	     }
	   else
	     {
	       //*m[row * (*y) + column] = 0;
	       pixel = SDL_MapRGB(image->format , 0 , 0 , 200);
	       put_pixel(image, row, column, pixel);
	     }
	   column++;
	 }
       row++;
       column = start_column;
     }
  
}





void extraction(SDL_Surface* image)
{
  int width = image -> w;
  int height = image -> h;

  int row = 0;
  int column = 0;
  //int *m;//Probably need to create pointers for each call 
  //So maybe it need to be move in the while
  //int *y;
  int x = 0;
  int y = 0;
  
    
    while (row < height/6)
    {
      printf("%i\n",height);
      printf("%i\n",width);
	while(column < width)
	  {
	    Uint32 pixel = get_pixel(image, column, row);
	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image->format, &r, &g, &b);
	    
	    if(r != 127)//Go through the image and when we find a character 
	      {
		printf("%i\n",column);
		 x = get_length_col(image,row, column, width, height);//First we get the length
		 y = get_length_row(image,row, column, width, height);
		//init_matrix();//Then we create a matrix with good dimensions
		//fill_matrix(image, row, column, x, y);//And then we fill the matrix with 0 - 1
		column += x;
		//pixel = SDL_MapRGB(image->format , 0 , 0 , 200);
	        //put_pixel(image, column, row, pixel);
	      }
	    column++;
	    //printf("%ui\n",column);
	  }
	row+= y;
	row ++;
	//printf("Row: %ui\n",row);//On saute à la prochaine ligne rouge
	column = 0;//Et on se replace au bord gauche de la page
	}
    //printf("%ui", *x);
    //printf("%ui", *y);
}








  
      
	  
