# ifndef filter_H
# define filter_H

void binarization(SDL_Surface* img);
void grayscale(SDL_Surface* img);
void applyingfilter(SDL_Surface* image);
double gaussian(int x,int y, double sigma);
void Buildkernel(size_t x , size_t y, double filter[x][y]);

#endif
