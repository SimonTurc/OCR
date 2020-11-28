#ifndef EXTRACT_H
#define EXTRACT_H

void extraction(SDL_Surface* image);
void get_length(SDL_Surface* image,int row,int col, int width, int height, int *x, int *y );
//void init_matrix();
void fill_matrix(SDL_Surface* image, int start_row, int start_colum);
#endif

