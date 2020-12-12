# ifndef filter_H
# define filter_H

Uint32 Otsu_Method(SDL_Surface* image);
void binarization(SDL_Surface* img, unsigned int otsu_value);
void grayscale(SDL_Surface* img);
void applying_filter(SDL_Surface* image, float kernel[]);
void median_filter(SDL_Surface* image);
void adjust_border(SDL_Surface* image);
void compute_filters(SDL_Surface* image,SDL_Surface* image_gaussian,SDL_Surface* image_median);
double var_histo(SDL_Surface *image);

#endif
