#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#define WIDTH 600
#define HEIGHT 400

// Variabili per pan e zoom
double offsetClickX = 0;
double offsetClickY = 0;
double offsetX = 0;
double offsetY = 0;
double zoom = 1.0;

// Funzione per disegnare la griglia
void draw_grid(cairo_t *cr)
{
    cairo_set_line_width(cr, 0.5);
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Colore grigio chiaro

    // Disegna linee orizzontali
    for (int i = 0; i <= 10; i++) {
        cairo_move_to(cr, offsetX, i * (HEIGHT / 10) + offsetY);
        cairo_line_to(cr, WIDTH + offsetX, i * (HEIGHT / 10) + offsetY);
    }

    // Disegna linee verticali
    for (int i = 0; i <= 10; i++) {
        cairo_move_to(cr, i * (WIDTH / 10) + offsetX, offsetY);
        cairo_line_to(cr, i * (WIDTH / 10) + offsetX, HEIGHT + offsetY);
    }

    cairo_stroke(cr);
}

// Funzione per disegnare il grafico
void draw_graph(cairo_t *cr)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 1); // Colore blu

    // Dati di esempio
    double x[10], y[10];
    for (int i = 0; i < 10; i++) {
        x[i] = i;
        y[i] = sin(i); // Funzione seno
    }

    // Inizia a disegnare il grafico
    cairo_move_to(cr, (x[0] * (WIDTH / 9) + offsetX) * zoom, (1 - (y[0] + 1) / 2) * HEIGHT * zoom + offsetY);
    for (int i = 1; i < 10; i++) {
        cairo_line_to(cr, (x[i] * (WIDTH / 9) + offsetX) * zoom, (1 - (y[i] + 1) / 2) * HEIGHT * zoom + offsetY);
    }
    cairo_stroke(cr);
}

// Funzione di callback per il disegno
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    draw_grid(cr);
    draw_graph(cr);
    return FALSE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il pan
static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        // Inizia il pan
        offsetClickX = event->x - offsetX;
        offsetClickY = event->y - offsetY;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

// Funzione per gestire il movimento del mouse
static gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
    if (event->state & GDK_BUTTON1_MASK) {
        // Aggiorna l'offset per il pan
        offsetX = event->x - offsetClickX;
        offsetY = event->y - offsetClickY;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

// Funzione per gestire lo zoom
static gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event)
{
    if (event->direction == GDK_SCROLL_UP) {
        zoom *= 1.1; // Zoom in
    } else if (event->direction == GDK_SCROLL_DOWN) {
        zoom /= 1.1; // Zoom out
    }
    gtk_widget_queue_draw(widget); // Ridisegna il widget
    return TRUE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Grafico con Cairo");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
    
    // Aggiungi gestori per il pan e lo zoom
    g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_button_press_event), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(on_motion_notify_event), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "scroll-event", G_CALLBACK(on_scroll_event), NULL);

    // Abilita gli eventi del mouse
    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
