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
void update_preview_cb (GtkFileChooser *file_chooser, gpointer data);


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
    image = GTK_WIDGET(gtk_builder_get_object(Builder, "image"));
    GtkWidget *preview;
    preview = gtk_image_new();
    gtk_file_chooser_set_preview_widget (GTK_FILE_CHOOSER(FileChooser), preview);
    g_signal_connect (FileChooser, "update-preview", G_CALLBACK (update_preview_cb), preview);
    
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(MainButton, "clicked", G_CALLBACK(on_MainButton_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void update_preview_cb (GtkFileChooser *file_chooser, gpointer data)
{
  GtkWidget *preview;
  char *filename;
  GdkPixbuf *pixbuf;
  gboolean have_preview;

  preview = GTK_WIDGET (data);
  filename = gtk_file_chooser_get_preview_filename (file_chooser);

  pixbuf = gdk_pixbuf_new_from_file_at_size (filename, 128, 128, NULL);
  have_preview = (pixbuf != NULL);
  g_free (filename);

  gtk_image_set_from_pixbuf (GTK_IMAGE (preview), pixbuf);
  if (pixbuf)
    g_object_unref (pixbuf);

  gtk_file_chooser_set_preview_widget_active (file_chooser, have_preview);
}

void on_MainButton_clicked()
{
    printf("main button clicked");
}

gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data)
{
    GdkPixbuf *pixbuf;
    GtkWidget* image = user_data;
    char *filename;

    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 750,750,NULL);
    g_free(filename);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);

    printf("File chosen! %s", filename);
    return FALSE;
}
