# ifndef skew_H
# define skew_H

double find_angle(SDL_Surface* image);
void angle_histogram(SDL_Surface* image, int histo[]);
double variance_histogram(int histo[], int height);
void replace_new_pixels(SDL_Surface* image);
    
    
#endif
