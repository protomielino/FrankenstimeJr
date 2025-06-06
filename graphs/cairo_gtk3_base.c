#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked: %s\n", (char *)data);
}

static void on_spin_button_changed(GtkWidget *spin_button, gpointer data) {
    gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
    g_print("Spin button value: %.2f\n", value);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Creazione della finestra principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Esempio Cairo + GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Creazione di una griglia
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Creazione di alcuni widget
    GtkWidget *label = gtk_label_new("Ciao, benvenuto!");
    GtkWidget *button1 = gtk_button_new_with_label("Pulsante 1");
    GtkWidget *button2 = gtk_button_new_with_label("Pulsante 2");
    GtkWidget *button3 = gtk_button_new_with_label("Pulsante 3");

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

    // Mostra tutti i widget
    gtk_widget_show_all(window);

    // Avvio del ciclo principale
    gtk_main();

    return 0;
}
