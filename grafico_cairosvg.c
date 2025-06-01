#include <cairo.h>
#include <cairo-svg.h>
#include <math.h>

#define WIDTH 600
#define HEIGHT 400

void draw_grid(cairo_t *cr) {
    cairo_set_line_width(cr, 0.5);
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Colore grigio chiaro

    // Disegna linee orizzontali
    for (int i = 0; i <= 10; i++) {
        cairo_move_to(cr, 0, i * (HEIGHT / 10));
        cairo_line_to(cr, WIDTH, i * (HEIGHT / 10));
    }

    // Disegna linee verticali
    for (int i = 0; i <= 10; i++) {
        cairo_move_to(cr, i * (WIDTH / 10), 0);
        cairo_line_to(cr, i * (WIDTH / 10), HEIGHT);
    }

    cairo_stroke(cr);
}

void draw_graph(cairo_t *cr) {
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

void draw_legend(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0, 0, 0); // Colore nero
    cairo_rectangle(cr, WIDTH - 120, 20, 100, 50);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 1, 1, 1); // Colore bianco
    cairo_move_to(cr, WIDTH - 110, 40);
    cairo_show_text(cr, "Grafico Seno");
}

int main() {
    cairo_surface_t *surface = cairo_svg_surface_create("grafico.svg", WIDTH, HEIGHT);
    cairo_t *cr = cairo_create(surface);

    // Disegna la griglia
    draw_grid(cr);
    // Disegna il grafico
    draw_graph(cr);
    // Disegna la leggenda
    draw_legend(cr);

    // Pulisci
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}
