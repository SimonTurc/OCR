#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *import_button;
GtkWidget *run_button;
GtkWidget *output_text;
GtkWidget *image1;
GtkWidget *input_file;
GtkBuilder *builder;

int is_image_loaded = 0;

void on_input_file_file_set(GtkFileChooserButton *f){
  gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) "");
  char *filename = (char*) gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
  int x = 70, y = 5;
  if (image1){
    gtk_container_remove(GTK_CONTAINER(fixed), image1);
  }
  image1 = gtk_image_new_from_file(filename);
  gtk_widget_show(image1);
  gtk_container_add(GTK_CONTAINER(fixed), image1);
  gtk_fixed_move(GTK_FIXED(fixed), image1, y, x);
  is_image_loaded = 1;
}

void on_run_button_clicked(GtkButton *b){
  if(image1 && is_image_loaded){
    gtk_container_remove(GTK_CONTAINER(fixed), image1);
    char text[] = "Là c'est censé afficher le texte mais on a pas fini le programme zebi";
    gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) text);
    is_image_loaded = 0;
  }
  if(!image1){
    gtk_label_set_text (GTK_LABEL(output_text), (const gchar*) "Choose an image");
  }
}

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("interface.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
  input_file = GTK_WIDGET(gtk_builder_get_object(builder, "input_file"));
  run_button = GTK_WIDGET(gtk_builder_get_object(builder, "run_button"));
  output_text = GTK_WIDGET(gtk_builder_get_object(builder, "output_text"));

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}
