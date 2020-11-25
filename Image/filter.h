# ifndef filter_H
# define filter_H

Uint32 Otsu_Method(SDL_Surface* image);
void binarization(SDL_Surface* img, unsigned int otsu_value);
void grayscale(SDL_Surface* img);
void applyingfilter(SDL_Surface* image);
double gaussian(int x,int y, double sigma);
void Buildkernel(size_t x , size_t y, double filter[x][y]);
void gaussian_filter(SDL_Surface* image);

#endif
