#include <gtk/gtk.h>

// Funzioni di conversione
void normalized_to_pixel(float normalized_x, float normalized_y, int window_width, int window_height, int *pixel_x, int *pixel_y) {
    *pixel_x = (int)(normalized_x * window_width);
    *pixel_y = (int)(normalized_y * window_height);
}

void pixel_to_normalized(int pixel_x, int pixel_y, int window_width, int window_height, float *normalized_x, float *normalized_y) {
    *normalized_x = (float)pixel_x / window_width;
    *normalized_y = (float)pixel_y / window_height;
}

// Funzione di esempio
void on_draw_event(GtkWidget *widget, cairo_t *cr) {
    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

    // Esempio di coordinate normalizzate
    float normalized_x = 0.5f; // 50% della larghezza
    float normalized_y = 0.5f; // 50% dell'altezza

    // Conversione in pixel
    int pixel_x, pixel_y;
    normalized_to_pixel(normalized_x, normalized_y, width, height, &pixel_x, &pixel_y);

    // Disegna un punto
    cairo_set_source_rgb(cr, 0, 0, 0); // Colore nero
    cairo_arc(cr, pixel_x, pixel_y, 5, 0, 2 * G_PI); // Disegna un cerchio
    cairo_fill(cr);
}

// Main
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "draw", G_CALLBACK(on_draw_event), NULL);

    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
