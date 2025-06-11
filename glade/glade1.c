// gcc -o glade1 glade1.c `pkg-config --cflags --libs gtk+-3.0 cairo` -rdynamic

#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *button1;
GtkWidget *label1;
GtkBuilder *builder; 

void on_button1_clicked(GtkButton *b)
{
    gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Hello World");
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade1.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
}
