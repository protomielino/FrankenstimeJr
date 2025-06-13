#include "graphics.h"

// Funzione per disegnare la griglia
static void draw_grid(cairo_t *cr, AppData *data)
{
    // griglia colore grigio
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 0.5);

    // Disegna linee orizzontali
    for (int i = data->grid_min_y; i <= data->grid_max_y; i++) {
        cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->offset_x, FLIP_VERTICAL((i * data->scale_y) + data->offset_y));
        cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->offset_x, FLIP_VERTICAL((i * data->scale_y) + data->offset_y));
    }

    // Disegna linee verticali
    for (int i = data->grid_min_x; i <= data->grid_max_x; i++) {
        cairo_move_to(cr, (i * data->scale_x) + data->offset_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->offset_y));
        cairo_line_to(cr, (i * data->scale_x) + data->offset_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->offset_y));
    }
    cairo_stroke(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);

    // Disegna il valore della coordinata verticale
    for (int i = data->grid_min_y; i <= data->grid_max_y; i++) {
        double y = (i * data->scale_y) + data->offset_y;

        cairo_set_source_rgba(cr, 0.2, 0.8, 0.2, 0.75); // verde
        cairo_move_to(cr, 10, FLIP_VERTICAL(y+5));
        cairo_show_text(cr, g_strdup_printf("%d", i));
    }

    int align = ALIGN_MIDDLE;

    // Disegna il valore della coordinata orizzontale
    for (int i = data->grid_min_x; i <= data->grid_max_x; i++) {
        double x = (i * data->scale_x) + data->offset_x;

        char text[10];
        snprintf(text, sizeof(text), "%d", i);
        cairo_text_extents_t text_extents;
        cairo_text_extents(cr, text, &text_extents);

        cairo_set_source_rgba(cr, 0.8, 0.2, 0.2, 0.75); // rosso
        switch(align) {
        case 1:
            // Allineamento a sinistra
            cairo_move_to(cr, x + 10, data->drawing_area_height - 15);
            break;
        case 2:
            // Allineamento a destra
            cairo_move_to(cr, x - text_extents.width + 10, data->drawing_area_height - 15);
            break;
        case 3:
            // Allineamento al centro
            cairo_move_to(cr, x - text_extents.width / 2 + 10, data->drawing_area_height - 15);
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
    cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->offset_x, FLIP_VERTICAL((0 * data->scale_y) + data->offset_y));
    cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->offset_x, FLIP_VERTICAL((0 * data->scale_y) + data->offset_y));
    cairo_stroke(cr);

    // asse verticale verde
    cairo_set_source_rgb(cr, 0.0, 0.8, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, (0 * data->scale_x) + data->offset_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->offset_y));
    cairo_line_to(cr, (0 * data->scale_x) + data->offset_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->offset_y));
    cairo_stroke(cr);
}

// Funzione per disegnare il grafico
static void draw_graph(cairo_t *cr, AppData *data)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 1); // Colore blu

    telemetryData *td = data->tel->data;

    float td0time = td[1].time;
    float td0distance = td[1].distance;
    if (td0time > 0 &&
        td0time < data->drawing_area_width &&
        td0distance > 0 &&
        td0distance < data->drawing_area_height)
    {
        cairo_move_to(cr,
                (td0time * data->scale_x) + data->offset_x,
                FLIP_VERTICAL((td0distance * data->scale_y) + data->offset_y));
    }

    for (size_t i = 2; i < data->tel->num_samples; i++) {
        float tdItime = td[i].time;
        float tdIdistance = td[i].distance;
        if (tdItime     * data->scale_x + data->offset_x > 0 &&
            tdItime     * data->scale_x + data->offset_x < data->drawing_area_width &&
            tdIdistance * data->scale_y + data->offset_y > 0 &&
            tdIdistance * data->scale_y + data->offset_y < data->drawing_area_height)
        {
            cairo_line_to(cr,
                    tdItime * data->scale_x + data->offset_x,
                    FLIP_VERTICAL(tdIdistance * data->scale_y + data->offset_y));
        }
    }
    cairo_stroke(cr);
}

// Funzione di callback per il disegno
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, AppData *data)
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
        cairo_rectangle(cr,
                data->select_start_x,
                data->select_start_y,
                data->select_end_x - data->select_start_x,
                data->select_end_y - data->select_start_y);
        cairo_fill(cr);
    }

    cairo_set_source_rgb(cr, 1, 1, 1);

    return TRUE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il movimento del mouse
gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, AppData *data)
{
    if (data->panning && (event->state & GDK_BUTTON1_MASK)) {
        // Aggiorna l'offset per il pan
        data->offset_x = event->x - data->offset_click_X;
        data->offset_y = FLIP_VERTICAL(event->y) + FLIP_VERTICAL(data->offset_click_Y);
        gtk_widget_queue_draw(widget);
    }

    if (data->selecting && (event->state & GDK_BUTTON1_MASK)) {
        data->select_end_x = event->x;
        data->select_end_y = event->y;
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }

    data->mouse_pos_X = event->x;
    data->mouse_pos_Y = event->y;

    double grid_x = (event->x - data->offset_x) / data->scale_x;
    double grid_y = (data->drawing_area_height - event->y - data->offset_y) / data->scale_y; // Inverti Y

    gtk_label_set_text (GTK_LABEL(data->widgets.label),
            g_strdup_printf("time:%.4f\tdistance:%.4f", grid_x, grid_y));

    return TRUE;
}

// Funzione per gestire il pan
gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, AppData *data)
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
//            g_print("Ctrl + Click: Azione 1\n");
        } else if (shift_pressed) {
            data->zooming = FALSE;
            data->panning = FALSE;
            data->selecting = TRUE;
//            g_print("Shift + Click: Azione 2\n");
        } else if (data->space_pressed) {
            data->zooming = FALSE;
            data->panning = TRUE;
            data->selecting = FALSE;
//            g_print("Space + Click: Azione 3\n");
        } else {
            data->zooming = FALSE;
            data->panning = TRUE;
            data->selecting = FALSE;
//            g_print("Click normale: Azione predefinita\n");
        }
    }

    if (data->selecting && (event->button == GDK_BUTTON_PRIMARY)) {
        data->selecting = TRUE;

        data->select_start_x = (data->scale_x - data->mouse_pos_X + data->offset_x) / data->scale_x;
        data->select_start_y = (data->scale_y - data->mouse_pos_Y + data->offset_y) / data->scale_y;
        data->select_start_x = event->x;
        data->select_start_y = event->y;
        data->select_end_x = event->x;
        data->select_end_y = event->y;

        gtk_widget_queue_draw(widget);
    }
    // Inizia il pan
    data->offset_click_X = event->x - data->offset_x;
    data->offset_click_Y = FLIP_VERTICAL(event->y) + FLIP_VERTICAL(data->offset_y);

    return TRUE;
}

// Funzione per gestire il rilascio del pulsante del mouse
gboolean on_button_release_event(GtkWidget *widget, GdkEventButton *event, AppData *data)
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
gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        data->space_pressed = TRUE;
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
gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        data->space_pressed = FALSE;
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

gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, AppData *data)
{
    double zoom_factor = 1.1; // Fattore di zoom
    double mouse_x = event->x;
    double mouse_y = event->y;

    // Calcola le coordinate della griglia prima dello zoom
    double grid_x = (mouse_x - data->offset_x) / data->scale_x;
    double grid_y = (data->drawing_area_height - mouse_y - data->offset_y) / data->scale_y; // Inverti Y

    if (event->direction == GDK_SCROLL_UP) {
        data->scale_x *= zoom_factor;
        data->scale_y *= zoom_factor;
    } else if (event->direction == GDK_SCROLL_DOWN) {
        data->scale_x /= zoom_factor;
        data->scale_y /= zoom_factor;
    }

    // Calcola il nuovo offset in modo che il punto sotto il mouse rimanga fisso
    data->offset_x = mouse_x - grid_x * data->scale_x;
    data->offset_y = (data->drawing_area_height - mouse_y) - grid_y * data->scale_y;

    gtk_widget_queue_draw(widget); // Richiesta di ridisegno

    return TRUE;
}
