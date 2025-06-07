#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#define WIDTH   800
#define HEIGHT  600
#define PAN

#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
#define ALIGN_MIDDLE 3

static gboolean space_pressed = FALSE;

typedef struct
{
    double scale;
    double offset_x;
    double offset_y;

    // Variabili per pan e zoom
    double mouse_pos_X;
    double mouse_pos_Y;
    double offset_click_X;
    double offset_click_Y;
    double scroll_origin_X;
    double scroll_origin_Y;
    double select_start_x;
    double select_start_y;
    double select_end_x;
    double select_end_y;
    gboolean panning;
    gboolean zooming;
    gboolean selecting;

    int drawing_area_width;
    int drawing_area_height;
} AppData;


// Funzione per disegnare la griglia
static void draw_grid(cairo_t *cr, AppData *data)
{
    // griglia colore grigio
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 0.5);

    // Disegna linee orizzontali
    for (int i = -10; i <= 10; i++) {
        cairo_move_to(cr, -(10 * data->scale) + data->offset_x, (i * data->scale) + data->offset_y);
        cairo_line_to(cr, (10 * data->scale) + data->offset_x, (i * data->scale) + data->offset_y);
    }

    // Disegna linee verticali
    for (int i = -10; i <= 10; i++) {
        cairo_move_to(cr, (i * data->scale) + data->offset_x, -(10 * data->scale) + data->offset_y);
        cairo_line_to(cr, (i * data->scale) + data->offset_x, (10 * data->scale) + data->offset_y);
    }
    cairo_stroke(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);

    // Disegna il valore della coordinata verticale
    for (int i = -10; i <= 10; i++) {
        double y = (i * data->scale) + data->offset_y;

        cairo_set_source_rgba(cr, 0.2, 0.8, 0.2, 0.75); // verde
        cairo_move_to(cr, 10, y + 5);
        cairo_show_text(cr, g_strdup_printf("%d", i));
    }

    int align = ALIGN_MIDDLE;

    // Disegna il valore della coordinata orizzontale
    for (int i = -10; i <= 10; i++) {
        double x = (i * data->scale) + data->offset_x;

        char text[10];
        snprintf(text, sizeof(text), "%d", i);
        cairo_text_extents_t text_extents;
        cairo_text_extents(cr, text, &text_extents);

        cairo_set_source_rgba(cr, 0.8, 0.2, 0.2, 0.75); // rosso
        switch(align) {
        case 1:
            // Allineamento a sinistra
            cairo_move_to(cr, x - 5, data->drawing_area_height - 15);
            break;
        case 2:
            // Allineamento a destra
            cairo_move_to(cr, x - text_extents.width - 5, data->drawing_area_height - 15);
            break;
        case 3:
            // Allineamento al centro
            cairo_move_to(cr, x - text_extents.width / 2, data->drawing_area_height - 15);
            break;
        default:
            break;
        }
        cairo_show_text(cr, text);
    }
    cairo_stroke(cr);

    // asse orizzontale rosso
    cairo_set_source_rgb(cr, 0.8, 0.0, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, -(10 * data->scale) + data->offset_x, (0 * data->scale) + data->offset_y);
    cairo_line_to(cr, (10 * data->scale) + data->offset_x, (0 * data->scale) + data->offset_y);
    cairo_stroke(cr);

    // asse verticale verde
    cairo_set_source_rgb(cr, 0.0, 0.8, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, (0 * data->scale) + data->offset_x, -(10 * data->scale) + data->offset_y);
    cairo_line_to(cr, (0 * data->scale) + data->offset_x, (10 * data->scale) + data->offset_y);
    cairo_stroke(cr);
}

typedef struct
{
    double x, y;
} point2d;

// Funzione per disegnare il grafico
void draw_graph(cairo_t *cr, AppData *data)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 1); // Colore blu

    // Dati di esempio
    point2d p[] = { {0, 0}, {1, 1} , {2, 01} };
#define MAX_POINTS (sizeof(p) / sizeof(p[0]))

    // Inizia a disegnare il grafico
    cairo_move_to(cr, p[0].x * data->scale + data->offset_x, p[0].y * data->scale + data->offset_y);

    for (int i = 1; i < MAX_POINTS; i++) {
        cairo_line_to(cr, p[i].x * data->scale + data->offset_x, p[i].y * data->scale + data->offset_y);
    }
    cairo_stroke(cr);
}

// Funzione di callback per il disegno
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, AppData *data)
{
    // Imposta il colore di sfondo a nero
    cairo_set_source_rgb(cr, 0, 0, 0); // Nero
    cairo_paint(cr); // Riempie l'area di disegno con il colore di sfondo

    data->drawing_area_width = gtk_widget_get_allocated_width(widget);
    data->drawing_area_height = gtk_widget_get_allocated_height(widget);

    draw_grid(cr, data);
    draw_graph(cr, data);

    // Disegna il rettangolo di selezione se attivo
    if (data->selecting) {
        cairo_set_source_rgba(cr, 0, 1, 0, 0.5); // verde semi-trasparente
        cairo_rectangle(cr, data->select_start_x, data->select_start_y, data->select_end_x - data->select_start_x, data->select_end_y - data->select_start_y);
        cairo_fill(cr);
    }

    cairo_move_to(cr, 10, 30);
    cairo_show_text(cr,
            (data->selecting ?
                    "selecting" :
                    data->panning ?
                            "panning" :
                            "")); // Scrivi il valore

    return TRUE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il movimento del mouse
static gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, AppData *data)
{
    if (data->panning) {
        if (event->state & GDK_BUTTON1_MASK) {
            // Aggiorna l'offset per il pan
            data->offset_x = event->x - data->offset_click_X;
            data->offset_y = event->y - data->offset_click_Y;
            gtk_widget_queue_draw(widget);
        }
    }

    if (data->selecting) {
        data->select_end_x = event->x;
        data->select_end_y = event->y;
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }

    data->mouse_pos_X = event->x;
    data->mouse_pos_Y = event->y;

    return TRUE;
}

// Funzione per gestire il pan
static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, AppData *data)
{
    // Controlla se il tasto Ctrl è premuto
    gboolean ctrl_pressed = (event->state & GDK_CONTROL_MASK) != 0;
    // Controlla se il tasto Shift è premuto
    gboolean shift_pressed = (event->state & GDK_SHIFT_MASK) != 0;

    if (event->type == GDK_BUTTON_PRESS) {
        if (ctrl_pressed) {
            data->zooming = TRUE;
            data->panning = FALSE;
            data->selecting = FALSE;
            g_print("Ctrl + Click: Azione 1\n");
        } else if (shift_pressed) {
            data->zooming = FALSE;
            data->panning = FALSE;
            data->selecting = TRUE;
            g_print("Shift + Click: Azione 2\n");
        } else if (space_pressed) {
            data->zooming = FALSE;
            data->panning = TRUE;
            data->selecting = FALSE;
            g_print("Space + Click: Azione 3\n");
        } else {
            data->zooming = FALSE;
            data->panning = TRUE;
            data->selecting = FALSE;
            g_print("Click normale: Azione predefinita\n");
        }
    }

    if (data->selecting) {
        if (event->button == GDK_BUTTON_PRIMARY) {
            data->selecting = TRUE;

            data->select_start_x = event->x;
            data->select_start_y = event->y;
            data->select_end_x = data->select_start_x;
            data->select_end_y = data->select_start_y;

            gtk_widget_queue_draw(widget);
        }
    }
    // Inizia il pan
    data->offset_click_X = event->x - data->offset_x;
    data->offset_click_Y = event->y - data->offset_y;

    return TRUE;
}

// Funzione per gestire il rilascio del pulsante del mouse
static gboolean on_button_release_event(GtkWidget *widget, GdkEventButton *event, AppData *data)
{
    if (event->button == GDK_BUTTON_PRIMARY) {
        data->zooming = FALSE;
        data->panning = FALSE;
        data->selecting = FALSE;
        // Qui puoi aggiungere la logica per gestire la selezione
        // Ad esempio, calcolare quali dati sono stati selezionati
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }
    return TRUE;
}

// Funzione per gestire il pan
static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        space_pressed = TRUE;
    }

//    if (event->hardware_keycode == GDK_KEY_space) {
//        data->panning = TRUE;
//    }
//    if (event->hardware_keycode == GDK_KEY_Control_L) {
//        data->zooming = TRUE;
//    }
//    if (event->hardware_keycode == GDK_KEY_Shift_L) {
//        data->selecting = TRUE;
//    }
    //    if (event->button == GDK_BUTTON_PRIMARY) {
    //        selecting = TRUE;
    //        select_start_x = event->x;
    //        select_start_y = event->y;
    //        select_end_x = event->x;
    //        select_end_y = event->y;
    //
    //#if 0
    //        // Inizia il pan
    //        offset_click_X = event->x - pan_X;
    //        offset_click_Y = event->y - pan_Y;
    //#endif
    //        gtk_widget_queue_draw(widget);
    //    }
    return TRUE;
}

// Funzione per gestire il rilascio del pulsante del mouse
static gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        space_pressed = FALSE;
    }

    data->panning = FALSE;
    data->zooming = FALSE;
    data->selecting = FALSE;
//    if (event->button == GDK_BUTTON_PRIMARY) {
//        selecting = FALSE;
//        // Qui puoi aggiungere la logica per gestire la selezione
//        // Ad esempio, calcolare quali dati sono stati selezionati
//        gtk_widget_queue_draw(widget); // Ridisegna il widget
//    }
    return TRUE;
}

static gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, AppData *data)
{
    double zoom_factor = 1.1; // Fattore di zoom
    double mouse_x = event->x;
    double mouse_y = event->y;

    // Calcola le coordinate della griglia prima dello zoom
    double grid_x = (mouse_x - data->offset_x) / data->scale;
    double grid_y = (mouse_y - data->offset_y) / data->scale;

    if (event->direction == GDK_SCROLL_UP) {
        data->scale *= zoom_factor;
    } else if (event->direction == GDK_SCROLL_DOWN) {
        data->scale /= zoom_factor;
    }

    // Calcola il nuovo offset in modo che il punto sotto il mouse rimanga fisso
    data->offset_x = mouse_x - grid_x * data->scale;
    data->offset_y = mouse_y - grid_y * data->scale;

    gtk_widget_queue_draw(widget); // Richiesta di ridisegno
    return TRUE;
}

int main(int argc, char *argv[])
{
    AppData data = {}; // Inizializza i dati
    data.drawing_area_width = 800;
    data.drawing_area_height = 600;

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Grafico Cairo");
    gtk_window_set_default_size(GTK_WINDOW(window), data.drawing_area_width, data.drawing_area_height);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    data.scale = 15.0;
    data.offset_x = data.drawing_area_width/2.0;
    data.offset_y = data.drawing_area_height/2.0; // Inizializza i dati

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press_event), &data);
    g_signal_connect(G_OBJECT(window), "key-release-event", G_CALLBACK(on_key_release_event), &data);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), &data);

    // Aggiungi gestori per il pan, lo zoom e la selezione
    g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(on_motion_notify_event), &data);
    g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_button_press_event), &data);
    g_signal_connect(G_OBJECT(drawing_area), "button-release-event", G_CALLBACK(on_button_release_event), &data);
    g_signal_connect(G_OBJECT(drawing_area), "scroll-event", G_CALLBACK(on_scroll_event), &data);

    // Abilita gli eventi del mouse
    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) |
            GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
