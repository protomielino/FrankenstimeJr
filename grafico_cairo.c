#include "grafico_cairo.h"

// Funzione per disegnare la griglia
void draw_grid(cairo_t *cr, telemetry *tel)
{
    cairo_set_line_width(cr, 0.5);
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Colore grigio chiaro

    int num_hLines = 10;
    int num_vLines = 10;

    // Disegna linee orizzontali
    for (int i = 0; i <= num_hLines; i++) {
        cairo_move_to(cr, 0, i * (HEIGHT / num_hLines));
        cairo_line_to(cr, WIDTH, i * (HEIGHT / num_hLines));
    }

    // Disegna linee verticali
    for (int i = 0; i <= num_vLines; i++) {
        cairo_move_to(cr, i * (WIDTH / num_vLines), 0);
        cairo_line_to(cr, i * (WIDTH / num_vLines), HEIGHT);
    }

    cairo_stroke(cr);
}

// Funzione per disegnare il grafico
void draw_graph(cairo_t *cr, telemetry *tel)
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
    cairo_move_to(cr, x[0] * (WIDTH / 9), (1 - (y[0] + 1) / 2) * HEIGHT);
    for (int i = 1; i < 10; i++) {
        cairo_line_to(cr, x[i] * (WIDTH / 9), (1 - (y[i] + 1) / 2) * HEIGHT);
    }
    cairo_stroke(cr);
}

// Funzione di callback per il disegno
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    telemetry *tel = (telemetry*)user_data;
    draw_grid(cr, tel);
    draw_graph(cr, tel);
    return FALSE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

//int main_1(int argc, char *argv[])
//{
//    gtk_init(&argc, &argv);
//
//    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    gtk_window_set_title(GTK_WINDOW(window), "Grafico con Cairo");
//    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
//    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
//
//    GtkWidget *drawing_area = gtk_drawing_area_new();
//    gtk_container_add(GTK_CONTAINER(window), drawing_area);
//    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
//
//    gtk_widget_show_all(window);
//    gtk_main();
//
//    return 0;
//}
