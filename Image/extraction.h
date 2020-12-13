#ifndef EXTRACTION_H
#define EXTRACTION_H

char extraction(SDL_Surface* image, int char_nb);
int get_length_col(SDL_Surface* image, int width,int col);
double* fill_matrix(SDL_Surface* image, int start_col, int end_col, double* m);
int count_char(SDL_Surface* image);
int count_space(SDL_Surface *image);
int addSpace(SDL_Surface *image,int char_nb);
#endif

