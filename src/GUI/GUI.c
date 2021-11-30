#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *image;
GtkWidget *MainButton;
GtkWidget *ClockW;
GtkWidget *CClockW;
GtkWidget *FileChooser;
GtkBuilder *Builder;

gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
void on_MainButton_clicked(); 

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    Builder = gtk_builder_new_from_file("GUI.glade");

    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    MainButton = GTK_WIDGET(gtk_builder_get_object(Builder, "MainButton"));
    ClockW = GTK_WIDGET(gtk_builder_get_object(Builder, "ClockW"));
    CClockW = GTK_WIDGET(gtk_builder_get_object(Builder, "CClockW"));
    FileChooser = GTK_WIDGET(gtk_builder_get_object(Builder, "FileChooser"));
    image = gtk_image_new();
    
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), &image);
    g_signal_connect(MainButton, "clicked", G_CALLBACK(on_MainButton_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void on_MainButton_clicked()
{
    printf("main button clicked");
}

gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data)
{
    GtkWidget* image = user_data;
    gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    gtk_image_set_from_file(GTK_IMAGE(image), filename);
    printf("File chosen!");
    return FALSE;
}
