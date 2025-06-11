// gcc -o glade1 glade1.c `pkg-config --cflags --libs gtk+-3.0 cairo` -rdynamic

#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *label2;
GtkBuilder *builder; 

void on_button1_clicked(GtkButton *b)
{
    char tmp[128];
    gdouble val = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin1));
    sprintf(tmp, "spin=%d", (int)val);
    gtk_label_set_text(GTK_LABEL(label1), (const gchar *) tmp);
}

void on_radio1_toggled(GtkRadioButton *b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Radio 1 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label2), (const gchar *) "Radio 1 Not Active");
    }
}

void on_radio2_toggled(GtkRadioButton *b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Radio 2 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label2), (const gchar *) "Radio 2 Not Active");
    }
}

void on_radio3_toggled(GtkRadioButton *b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Radio 3 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label2), (const gchar *) "Radio 3 Not Active");
    }
}

void on_check1_toggled(GtkCheckButton *b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Check 1 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Check 1 Not Active");
    }
}

void on_toggle1_toggled(GtkToggleButton *b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Toggle 1 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Toggle 1 Not Active");
    }
}

void on_switch1_state_set(GtkSwitch *s)
{
    gboolean T = gtk_switch_get_active(s);
    if (T) {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Switch 1 Active");
    } else {
        gtk_label_set_text(GTK_LABEL(label1), (const gchar *) "Switch 1 Not Active");
    }
}

void on_entry1_changed(GtkEntry *e)
{
    char tmp[128];
    sprintf(tmp, "entry=%s", gtk_entry_get_text(e));
    gtk_label_set_text(GTK_LABEL(label1), (const gchar *) tmp);
}
void on_combo1_changed(GtkComboBox *c)
{
    printf("combo1 changed entered\n");
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade2.glade");
    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    label2 = GTK_WIDGET(gtk_builder_get_object(builder, "label2"));
    radio1 = GTK_WIDGET(gtk_builder_get_object(builder, "radio1"));
    radio2 = GTK_WIDGET(gtk_builder_get_object(builder, "radio2"));
    radio3 = GTK_WIDGET(gtk_builder_get_object(builder, "radio3"));
    check1 = GTK_WIDGET(gtk_builder_get_object(builder, "check1"));
    toggle1 = GTK_WIDGET(gtk_builder_get_object(builder, "toggle1"));
    spin1 = GTK_WIDGET(gtk_builder_get_object(builder, "spin1"));
    combo1 = GTK_WIDGET(gtk_builder_get_object(builder, "combo1"));
    entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));

    gtk_widget_show(window);

    gtk_main();

    return EXIT_SUCCESS;
}
