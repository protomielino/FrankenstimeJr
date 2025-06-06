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
gboolean selecting = FALSE;
double select_start_x = 0;
double select_start_y = 0;
double select_end_x = 0;
double select_end_y = 0;

// Dichiarazione globale per il gruppo di radio button
GtkWidget *radio_button1;
GtkWidget *radio_button2;

// Funzione per gestire il cambiamento del radio button
void on_radio_button_toggled(GtkToggleButton *toggle_button, gpointer user_data)
{
    if (gtk_toggle_button_get_active(toggle_button)) {
        const char *label = gtk_button_get_label(GTK_BUTTON(toggle_button));
        g_print("Selezionato: %s\n", label);
        // Aggiungi qui la logica per gestire la selezione
    }
}

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

    // Disegna il rettangolo di selezione se attivo
    if (selecting) {
        cairo_set_source_rgba(cr, 0, 1, 0, 0.5); // Colore verde semi-trasparente
        cairo_rectangle(cr, select_start_x, select_start_y, select_end_x - select_start_x, select_end_y - select_start_y);
        cairo_fill(cr);
    }

    return FALSE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il pan
static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        selecting = TRUE;
        select_start_x = event->x;
        select_start_y = event->y;
        select_end_x = event->x;
        select_end_y = event->y;

#if 0
        // Inizia il pan
        offsetClickX = event->x - offsetX;
        offsetClickY = event->y - offsetY;
#endif
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

// Funzione per gestire il movimento del mouse
static gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
    if (selecting) {
        select_end_x = event->x;
        select_end_y = event->y;
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }
    return TRUE;
#if 0
    if (event->state & GDK_BUTTON1_MASK) {
        // Aggiorna l'offset per il pan
        offsetX = event->x - offsetClickX;
        offsetY = event->y - offsetClickY;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
#endif
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

// Funzione per gestire il rilascio del pulsante del mouse
static gboolean on_button_release_event(GtkWidget *widget, GdkEventButton *event)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        selecting = FALSE;
        // Qui puoi aggiungere la logica per gestire la selezione
        // Ad esempio, calcolare quali dati sono stati selezionati
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

// Creazione della finestra principale
GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "Grafico con Cairo");
gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

// Creazione di un contenitore verticale
GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

// Creazione dell'area di disegno per il grafico
GtkWidget *drawing_area = gtk_drawing_area_new();
gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

// Aggiunta di radio button e combo box al contenitore
GtkWidget *radio_button1 = gtk_radio_button_new_with_label(NULL, "Giro 1");
gtk_box_pack_start(GTK_BOX(vbox), radio_button1, FALSE, FALSE, 0);

GtkWidget *radio_button2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button1), "Giro 2");
gtk_box_pack_start(GTK_BOX(vbox), radio_button2, FALSE, FALSE, 0);

GtkWidget *combo_box = gtk_combo_box_new_with_entry();
gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "Giro 1");
gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "Giro 2");
gtk_box_pack_start(GTK_BOX(vbox), combo_box, FALSE, FALSE, 0);

// Aggiunta del contenitore alla finestra principale
gtk_container_add(GTK_CONTAINER(window), vbox);

// Collega i segnali per il disegno e gli eventi
g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_button_press_event), NULL);
g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(on_motion_notify_event), NULL);
g_signal_connect(G_OBJECT(drawing_area), "button-release-event", G_CALLBACK(on_button_release_event), NULL);
g_signal_connect(G_OBJECT(drawing_area), "scroll-event", G_CALLBACK(on_scroll_event), NULL);

    // Aggiungi qui i radio button e il combo box
 
    // Aggiunta dei radio button nel main
    radio_button1 = gtk_radio_button_new_with_label(NULL, "Giro 1");
    g_signal_connect(radio_button1, "toggled", G_CALLBACK(on_radio_button_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), radio_button1, FALSE, FALSE, 0);

    radio_button2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button1), "Giro 2");
    g_signal_connect(radio_button2, "toggled", G_CALLBACK(on_radio_button_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), radio_button2, FALSE, FALSE, 0);

// Mostra tutti i widget
gtk_widget_show_all(window);
gtk_main();


//    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    
    // Abilita gli eventi del mouse
//    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | 
//        GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK);

    return 0;
}

int main_0(int argc, char *argv[])
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
