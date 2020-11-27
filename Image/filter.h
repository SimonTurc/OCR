# ifndef filter_H
# define filter_H

Uint32 Otsu_Method(SDL_Surface* image);
void binarization(SDL_Surface* img, unsigned int otsu_value);
void grayscale(SDL_Surface* img);
void applying_filter(SDL_Surface* image, float kernel[]);

#endif
