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
GtkBuilder *builder;

void on_import_button_clicked(GtkButton *b){
  printf("la grosse moula");
  gtk_label_set_text(GTK_LABEL(output_text), (const gchar* ) "Shifter Pro");
}

void on_run_button_clicked(GtkButton *b){
  printf("la grosse moula");
  gtk_label_set_text(GTK_LABEL(output_text), (const gchar* ) "Gros chibrax d'enculé de fils de pute ta mère elle suce des glands et elle se fait prendre en levrette par pleins de noirs en chaleur");
}

int main(int argc, char *argv[]){
  printf("la grosse moulaga1311321");
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("interface.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
  import_button = GTK_WIDGET(gtk_builder_get_object(builder, "import_button"));
  run_button = GTK_WIDGET(gtk_builder_get_object(builder, "run_button"));
  output_text = GTK_WIDGET(gtk_builder_get_object(builder, "output_text"));

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}
