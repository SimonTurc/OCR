# ifndef segmentation_H
# define segmentation_H

void horizontal_histogram(SDL_Surface* image);
void vertical_histogram(SDL_Surface* image);
SDL_Surface* cut_image(SDL_Surface* image, int nb_image);
SDL_Surface* cut_lines(SDL_Surface* image, int first, int last);
int number_of_lines(SDL_Surface* image);
    
#endif
