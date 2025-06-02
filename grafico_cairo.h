#ifndef GRAFICO_CAIRO_H_
#define GRAFICO_CAIRO_H_

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#include "telemetry.h"

#define WIDTH 600
#define HEIGHT 400

// Funzione per disegnare la griglia
void draw_grid(cairo_t *cr, telemetry *tel);
// Funzione per disegnare il grafico
void draw_graph(cairo_t *cr, telemetry *tel);
// Funzione di callback per il disegno
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);

#endif /* GRAFICO_CAIRO_H_ */
