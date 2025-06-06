#include <gtk/gtk.h>

static gdouble mouse_posX = 0.0;
static gdouble mouse_posY = 0.0;

static void on_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Button clicked: %s\n", (char *)data);
}

static void on_spin_button_changed(GtkWidget *spin_button, gpointer data)
{
    gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
    g_print("Spin button value: %.2f\n", value);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_move_to(cr, 10, 15);
    cairo_show_text(cr, "Hello, GTK3 with Cairo!");
    return FALSE;
}

static gboolean on_mouse_move(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    mouse_posX = event->x;
    mouse_posY = event->y;

//    g_print("Mouse moved to: (%.2f, %.2f)\n", event->x, event->y);
    return FALSE;
}

static gboolean on_mouse_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1) {
        g_print("Left mouse button pressed at: (%.2f, %.2f)\n", event->x, event->y);
    } else if (event->button == 3) {
        g_print("Right mouse button pressed at: (%.2f, %.2f)\n", event->x, event->y);
    }
    return FALSE;
}
static gboolean on_mouse_released(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        g_print("Left mouse button released at: (%.2f, %.2f)\n", event->x, event->y);
    } else if (event->button == GDK_BUTTON_SECONDARY) {
        g_print("Right mouse button released at: (%.2f, %.2f)\n", event->x, event->y);
    }
    printf("ciao\n");
    return FALSE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Creazione della finestra principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Esempio Cairo + GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Creazione di una griglia
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(vbox), grid);

    // Creazione di alcuni widget
    GtkWidget *label = gtk_label_new("Ciao, benvenuto!");
    GtkWidget *button1 = gtk_button_new_with_label("Pulsante 1");
    GtkWidget *button2 = gtk_button_new_with_label("Pulsante 2");
    GtkWidget *button3 = gtk_button_new_with_label("Pulsante 3");
    GtkWidget *drawing_area;

    // Creazione di uno SpinButton per valori numerici
    GtkWidget *spin_button = gtk_spin_button_new_with_range(0, 100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button), 50); // Valore iniziale

    // Aggiunta dei widget alla griglia
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1); // (griglia, widget, colonna, riga, larghezza, altezza)
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), spin_button, 0, 3, 2, 1); // Aggiunta dello SpinButton

    // Connessione dei segnali per i pulsanti
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), "Pulsante 1");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "Pulsante 2");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button_clicked), "Pulsante 3");
    g_signal_connect(spin_button, "value-changed", G_CALLBACK(on_spin_button_changed), NULL);

    // Area di disegno
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 200);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(on_mouse_move), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_mouse_pressed), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "button-release-event", G_CALLBACK(on_mouse_released), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    // Abilita la ricezione degli eventi del mouse
    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

    // Mostra tutti i widget
    gtk_widget_show_all(window);

    // Avvio del ciclo principale
    gtk_main();

    return 0;
}
