#ifndef EXTRACTION_H
#define EXTRACTION_H

void extraction(SDL_Surface* image);
int get_length_col(SDL_Surface* image,int row,int col, int width, int height);
int get_length_row(SDL_Surface* image,int row,int col,int width, int height);
//void init_matrix();
void fill_matrix(SDL_Surface* image, int start_row, int start_colum, int *x);
#endif

