#include "image_treatment.h"
#include <SDL/SDL_rotozoom.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *image;
GtkWidget *MainButton;
GtkWidget *ClockW;
GtkWidget *CClockW;
GtkWidget *FileChooser;
GtkBuilder *Builder;
char* filename;
SDL_Surface * Loaded = NULL;


gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
void on_MainButton_clicked(); 
void update_preview_cb (GtkFileChooser *file_chooser, gpointer data);
void on_MainButton_clicked(gpointer data);
void clockwise(gpointer data);
void cclockwise(gpointer data);
int main_treat();



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
    g_signal_connect (ClockW, "clicked", G_CALLBACK(clockwise),image);
    g_signal_connect (CClockW, "clicked", G_CALLBACK(cclockwise),image);
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(MainButton, "clicked", G_CALLBACK(on_MainButton_clicked), image);

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

void on_MainButton_clicked(gpointer data)
{
    GtkWidget *image = data;
    GdkPixbuf *pixbuf;
    if (filename != NULL)
    {
        main_treat();
        // No file chosen wip

        pixbuf = gdk_pixbuf_new_from_file_at_size("splitting.bmp", 750,750,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    

        if (pixbuf)
            g_object_unref(pixbuf);
    }
}

void clockwise(gpointer data)
{
    GtkWidget *image = data;
    if (image != NULL)
    {
        GdkPixbuf *pixbuf;

        Loaded = rotozoomSurface(Loaded, -90, 1, 1);

        SDL_SaveBMP(Loaded, "rotated.bmp");

        pixbuf = gdk_pixbuf_new_from_file_at_size("rotated.bmp", 750,750,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    

        if (pixbuf)
            g_object_unref(pixbuf);

    }
}

void cclockwise(gpointer data)
{
    GtkWidget *image = data;
    if (image != NULL)
    {
        GdkPixbuf *pixbuf;

        Loaded = rotozoomSurface(Loaded, 90, 1, 1);

        SDL_SaveBMP(Loaded, "rotated.bmp");

        pixbuf = gdk_pixbuf_new_from_file_at_size("rotated.bmp", 750,750,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);


        if (pixbuf)
            g_object_unref(pixbuf);

    }
}


gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data)
{
    GdkPixbuf *pixbuf;
    GtkWidget* image = user_data;

    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    Loaded = IMG_Load(filename);
    pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 750,750,NULL);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);

    printf("File chosen! %s", filename);
    return FALSE;
}


int main_treat()
{

    //The surface of the image we are gonna load
    SDL_Surface * Loaded = NULL;

    //The surface for sobel
    SDL_Surface *sobel_surface=NULL;


    //Initialize SDL video module
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        if(!Loaded) 
        {
            printf("IMG_Load: %s\n", IMG_GetError());
        }
        else
        {

            //Treat the loaded image
            //
            //Grayscale
            grayscale(Loaded);

            //Median filter
            MedianFilter(Loaded);

            //Gamma
            Gamma(Loaded);

            //Median filter
            MedianFilter(Loaded);

            //Black and White
            colorTreatment(Loaded, 242);

            //Median filter
            MedianFilter(Loaded);
            /*
            //Sobel
            sobel_surface= SDL_CreateRGBSurface(0,Loaded->w, Loaded->h, 32, 0,0,0,0);
            SobelEdgeDetection(Loaded, sobel_surface, 0.02);
            SDL_BlitSurface(sobel_surface, NULL, screenSurface, NULL);
            SDL_Flip(screenSurface);
            wait_for_keypressed();
            */

            // double angle = HoughTransformAngleDetection(sobel_surface, Loaded, 180, 180, 3);//, "blue");

            //To black
            colorTreatment(Loaded, 30);


            //Median filter
            MedianFilter(Loaded);

            // Rotation and update with hough angle
            // Loaded  = rotozoomSurface(screenSurface, angle, 1, 1);


            SDL_SaveBMP(Loaded, "Splitting.bmp");

            /*
            //Gamma
            Gamma(sobel_surface);
            SDL_BlitSurface(sobel_surface, NULL, screenSurface, NULL);
            SDL_Flip(screenSurface);
            wait_for_keypressed();


            //Black and White
            colorTreatment(sobel_surface, 242);
            SDL_SaveBMP(sobel_surface, "BlackAndWhite.bmp");
            SDL_BlitSurface(sobel_surface, NULL, screenSurface, NULL);
            SDL_Flip(screenSurface);
            wait_for_keypressed();*/

            //Wait for a key to be pressed to end the program
            wait_for_keypressed();
            
        }
    }
    SDL_FreeSurface( Loaded );
    SDL_FreeSurface( sobel_surface );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
