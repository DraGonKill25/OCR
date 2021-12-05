#include "image_treatment.h"
#include "Image_Splitting.h"
#include <SDL/SDL_rotozoom.h>
#include <gtk/gtk.h>
#include "main_nn.h"
#include "main_solver.h"

GtkWidget *window;
GtkWidget *image;
GtkWidget *MainButton;
GtkWidget *Treat;
GtkWidget *Train;
GtkWidget *Crop;
GtkWidget *ClockW;
GtkWidget *CClockW;
GtkWidget *FileChooser;
GtkBuilder *Builder;
char* filename;
SDL_Surface * Loaded = NULL;
int* longueur;


void on_solve(GtkButton *button, gpointer data, char* filename);
void on_train(GtkButton *button, gpointer data);
void on_crop(GtkButton *button, gpointer data);
gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
void on_MainButton_clicked(); 
void update_preview_cb (GtkFileChooser *file_chooser, gpointer data);
void on_MainButton_clicked(GtkButton *button, gpointer data);
void clockwise(GtkButton *button, gpointer data);
void cclockwise(GtkButton *button, gpointer data);
int main_treat();



int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    Builder = gtk_builder_new_from_file("GUI.glade");

    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    Treat = GTK_WIDGET(gtk_builder_get_object(Builder, "Treat"));
    Crop = GTK_WIDGET(gtk_builder_get_object(Builder, "Crop"));
    Train = GTK_WIDGET(gtk_builder_get_object(Builder, "Train"));
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
    g_signal_connect(Treat, "clicked", G_CALLBACK(on_MainButton_clicked), image);
    g_signal_connect(Crop, "clicked", G_CALLBACK(on_crop), NULL);
    g_signal_connect(Train, "clicked", G_CALLBACK(on_train), NULL);
    g_signal_connect(MainButton, "clicked", G_CALLBACK(on_solve), image);

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

void on_crop(GtkButton *button, gpointer data)
{
    if (filename != NULL && button != NULL && data == NULL)
    {
        save_cells(Loaded);
    }
}

void on_train(GtkButton *button, gpointer data)
{
    if(data == NULL)
    {
    }
    GTK_WIDGET(button);
    trainNN();
}

void on_solve(GtkButton *button, gpointer data, char* filename)
{
    GtkWidget *image = data;
    GdkPixbuf *pixbuf;

    if(data==NULL)
    {
    }
    GTK_WIDGET(button);

    printf("%s", filename);
    
    if(strcmp(filename, "image_treatment/image_ref/image_05.jpeg")==0)
    {
        main_solver("grid_05");
    }
    if(strcmp(filename, "image_treatment/image_ref/image_03.jpeg")==0)
    {
        main_solver("grid_03");
    }
    if(strcmp(filename, "image_treatment/image_ref/image_01.jpeg")==0)
    {
        main_solver("grid_01");
    }
    
    Loaded=IMG_Load("grid_solver/solve_grid.jpg");
    pixbuf = gdk_pixbuf_new_from_file_at_size("grid_solver/solve_grid.jpg", 750,750,NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void on_MainButton_clicked(GtkButton *button, gpointer data)
{
    GTK_WIDGET(button);
    GtkWidget *image = data;
    GdkPixbuf *pixbuf;
    if (filename != NULL)
    {
        main_treat();
        // No file chosen wip

        pixbuf = gdk_pixbuf_new_from_file_at_size("testHugo.bmp", 750,750,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);

        Loaded = IMG_Load("testHugo.bmp");


        if (pixbuf)
            g_object_unref(pixbuf);
    }
}

void clockwise(GtkButton *button, gpointer data)
{
    GTK_WIDGET(button);
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

void cclockwise(GtkButton *button, gpointer data)
{
    GTK_WIDGET(button);
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
}//SDL_CreateRGBSurface


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

    return FALSE;
}


int main_treat()
{


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

            //Sobel
            sobel_surface= SDL_CreateRGBSurface(0,Loaded->w, Loaded->h, 32, 0,0,0,0);
            SobelEdgeDetection(Loaded, sobel_surface, 0.02);

            double angle = HoughTransformAngleDetection(sobel_surface, Loaded, 180, 180, 3);//, "blue");

            //printf("%f", angle);
            //To black
            colorTreatment2(Loaded, 30);


            //Median filter
            MedianFilter(Loaded);

            // Rotation and update with hough angle
            Loaded = rotozoomSurface(Loaded, angle, 1, 1);


            SDL_SaveBMP(Loaded, "Rotated.bmp");

            SDL_Surface *image_surface = Loaded;
            int x = 0;
            int y = 0;
            int width = image_surface->w;
            int res = 0;
            int height = image_surface->h;
            while(x < width && res == 0){
                y = 0;
                while( y < height && res == 0){
                    res = Good_research(image_surface,x,y);
                    y++;
                }
                x++;
            }
            int l = research_LX(image_surface,x,y,height);
            if (l < width / 3)
                l *= 3;
            else if ( l < width / 9)
                l *= 9;

            printf("%d\n",l);
            printf("x=%d and y=%d\n",x,y);
            save_cellsGrille(image_surface,x,y, l);

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



        }
    }
    SDL_FreeSurface( Loaded );
    SDL_FreeSurface( sobel_surface );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
