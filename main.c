#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>
#include <err.h>
#include <stdlib.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "Image/basics/sdl_basics.h"
#include "Image/filter.h"
#include "Image/segmentation.h"
#include "Image/extraction.h"
#include "Image/matrix_resize.h"
#include "Image/skew.h"
#include "text.h"
#include "get_text.h"

#define CONTRAST 150
#define PATH_SIZE 256

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *import_button;
GtkWidget *run_button;
GtkWidget *output_text;
GtkWidget *image1;
GtkWidget *input_file;
GtkWidget *toggle_button;
GtkWidget *radio_none;
GtkWidget *radio2;
GtkWidget *radio3;
GtkBuilder *builder;

int is_image_loaded = 0;

void on_input_file_file_set(GtkFileChooserButton *f){
  gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) "");
  char *filename = (char*) gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
  FILE* output_file = fopen("output_text.txt", "w+");
  if (output_text == NULL){
    errx(1, "can't open output_file");
  }
  fputs((const char*)filename, output_file);
  fclose(output_file);
  int x = 116, y = 5;
  if (image1){
    gtk_container_remove(GTK_CONTAINER(fixed), image1);
  }
  image1 = gtk_image_new_from_file(filename);
  gtk_widget_show(image1);
  gtk_container_add(GTK_CONTAINER(fixed), image1);
  gtk_fixed_move(GTK_FIXED(fixed), image1, y, x);
  is_image_loaded = 1;
}

void on_run_button_clicked(){
  if(image1 && is_image_loaded){
    gtk_container_remove(GTK_CONTAINER(fixed), image1);
    int text_size = get_text();
    char text[text_size];
    char textline[text_size];
    FILE* output_file = fopen("output_text.txt", "r");
    if (output_text == NULL){
      errx(1, "can't open output_file");
    }
    int i = 0;
    while(fgets(textline, text_size, output_file) != NULL){
      if (i!=0){
	strcat(text, textline);
      }
      i++;
    }
    fclose(output_file);
    gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) text);
    is_image_loaded = 0;
  }
  if(!image1){
    gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) "Choose an image");
  }
}

int get_text()
{
    SDL_Surface *image;
    SDL_Surface *image_median;
    SDL_Surface *image_gaussian;
    
    SDL_Surface *image_rotate;
    SDL_Surface *line;
    
    double angle = 0;
    int nb_lines = 0;
    int char_per_line = 0;
    unsigned int otsu_value = 0;

    FILE* output_file = fopen("output_text.txt", "r");
    if (output_text == NULL){
      errx(1, "can't open output_file");
    }
    char filename[PATH_SIZE] = "";
    if (fgets(filename, PATH_SIZE, output_file) == NULL)
    {
	errx(1, "can't open output_file");;
    }
    fclose(output_file);
    
    int nb_char = strlen(filename) + 2;
    
    image = load_image(filename);
    image_gaussian = load_image(filename);
    image_median = load_image(filename);

    contrast_adjustment(image, CONTRAST);
    contrast_adjustment(image_gaussian, CONTRAST);
    contrast_adjustment(image_median, CONTRAST);
    
    init_sdl();

    grayscale(image_median);
    grayscale(image_gaussian);
    grayscale(image);
    
    median_filter(image_median);
    applying_filter(image_gaussian);

    if (gtk_toggle_button_get_active ((GtkToggleButton*)toggle_button)){
      thickness(image);
    }

    if (gtk_toggle_button_get_active ((GtkToggleButton*)radio2)){
      noise_reduction(image);
    }
    
    otsu_value = Otsu_Method(image) +30;
    binarization(image, otsu_value);
    binarization(image_median, otsu_value);
    binarization(image_gaussian, otsu_value);

    if (gtk_toggle_button_get_active ((GtkToggleButton*)radio2)){
      angle = find_angle(image);
      image_rotate = rotozoomSurface(image, angle, 1.0, 0);
      replace_new_pixels(image_rotate);
    }

    if (gtk_toggle_button_get_active ((GtkToggleButton*)radio3)){
      compute_filters(image, image_gaussian, image_median);
      if (var_histo(image_median) > var_histo(image))
	{
	  angle = find_angle(image_median);
	  image_rotate = rotozoomSurface(image_median, angle, 1.0, 0);
	  replace_new_pixels(image_rotate);
	}
      else
	{
	  angle = find_angle(image);
	  image_rotate = rotozoomSurface(image, angle, 1.0, 0);
	  replace_new_pixels(image_rotate);
	}
    }
    else{
      angle = find_angle(image);
      image_rotate = rotozoomSurface(image, angle, 1.0, 0);
      replace_new_pixels(image_rotate);
    }
    
    horizontal_histogram(image_rotate);
    nb_lines = number_of_lines(image_rotate);
    
    output_file = fopen("output_text.txt", "a+");
    if (output_text == NULL){
      errx(1, "can't open output_file");
    }
    fputc((int)'\n', output_file);
    
    for (int i = 1; i <= nb_lines; i++)
    {
        line = cut_image(image_rotate, i);
	/*if (i == 1)
	{
	     SDL_SaveBMP(line, "out.bmp");
	}*/
        vertical_histogram(line);
        char_per_line = count_char(line);
        for (int j = 1; j <= char_per_line; j++)
        {
	  fputc((int) extraction(line, j), output_file);
	  nb_char += 1;
	  if(addSpace(line,j) == 1){
	    fputc(32, output_file);
	    nb_char += 1;
	  }
        }
	nb_char += 1;
	fputc((int)'\n', output_file);
        SDL_FreeSurface(line);
    }
    fclose(output_file);
    SDL_FreeSurface(image);
    SDL_FreeSurface(image_gaussian);
    SDL_FreeSurface(image_median);
    SDL_FreeSurface(image_rotate);
    return nb_char;
}


int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);
  builder = gtk_builder_new_from_file("Interface/interface.glade");
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
  input_file = GTK_WIDGET(gtk_builder_get_object(builder, "input_file"));
  run_button = GTK_WIDGET(gtk_builder_get_object(builder, "run_button"));
  output_text = GTK_WIDGET(gtk_builder_get_object(builder, "output_text"));
  toggle_button = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_button"));
  radio_none = GTK_WIDGET(gtk_builder_get_object(builder, "radio_none"));
  radio2 = GTK_WIDGET(gtk_builder_get_object(builder, "radio_noise"));
  radio3 = GTK_WIDGET(gtk_builder_get_object(builder, "radio_decision"));

  gtk_widget_show(window);
  gtk_main();
  return EXIT_SUCCESS;
}
