#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Button clicked!\n");
}

static void on_checkbox_toggled(GtkToggleButton *togglebutton, gpointer data)
{
    gboolean active = gtk_toggle_button_get_active(togglebutton);
    g_print("Checkbox is %s\n", active ? "active" : "inactive");
}

static void on_combo_changed(GtkComboBox *combo, gpointer data)
{
    gchar *active_text = gtk_combo_box_get_active_text(combo);
    g_print("Selected: %s\n", active_text);
    g_free(active_text);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_move_to(cr, 10, 50);
    cairo_show_text(cr, "Hello, GTK2 with Cairo!");
    return FALSE;
}

int main_1(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;
    GtkWidget *checkbox;
    GtkWidget *combo;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Esempio GTK2 con Cairo");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    button = gtk_button_new_with_label("Clicca qui");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    checkbox = gtk_check_button_new_with_label("Attiva/Disattiva");
    g_signal_connect(checkbox, "toggled", G_CALLBACK(on_checkbox_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), checkbox, FALSE, FALSE, 0);

    combo = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Opzione 1");
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Opzione 2");
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Opzione 3");
    g_signal_connect(combo, "changed", G_CALLBACK(on_combo_changed), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 200);
    g_signal_connect(G_OBJECT(drawing_area), "expose-event", G_CALLBACK(on_draw_event), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
