#include <math.h>

#include "graphics.h"
#include "telemetry.h"

#include "stb_ds.h"

// Funzione per disegnare la griglia
static void draw_grid(cairo_t *cr, AppData *data)
{
#if 0
    double grid_border_left, grid_border_right;
    double grid_border_top, grid_border_bottom;
    double grid_min_x, grid_min_y;
    double grid_max_x, grid_max_y;

    grid_border_left = 30;
    grid_border_top = 5;
    grid_min_x    = (grid_border_left - data->pan_x) / data->scale_x;
    grid_min_y    = (data->drawing_area_height - grid_border_top - data->pan_y) / data->scale_y; // Inverti Y
    data->grid_min_x = grid_min_x;
    data->grid_min_y = grid_min_y;

    grid_border_right = data->drawing_area_width - 5;
    grid_border_bottom = data->drawing_area_height - 30;
    grid_max_x    = (grid_border_right - data->pan_x) / data->scale_x;
    grid_max_y    = (data->drawing_area_height - grid_border_bottom - data->pan_y) / data->scale_y; // Inverti Y
    data->grid_max_x = grid_max_x;
    data->grid_max_y = grid_max_y;

//    int grid_extension_x, grid_extension_y;
//    grid_extension_x = fabsf(data->grid_max_x - data->grid_min_x);
//    grid_extension_y = fabsf(data->grid_max_y - data->grid_min_y);
//    int grid_spacing_x, grid_spacing_y;
//    grid_spacing_x = pow(10, (int)log10(grid_extension_x));
//    grid_spacing_y = pow(10, (int)log10(grid_extension_y));

    // Calcola la densità delle linee in base alla scala
    double grid_density_x = 1.0 / data->scale_x; // Densità delle linee verticali
    double grid_density_y = 1.0 / data->scale_y; // Densità delle linee orizzontali

    // Calcola gli intervalli per le linee della griglia
    double vertical_spacing = fmax(1, (int)(grid_density_x * 10)); // Almeno 1 unità di distanza
    double horizontal_spacing = fmax(1, (int)(grid_density_y * 10)); // Almeno 1 unità di distanza

    // Disegna linee orizzontali
    for (int i = data->grid_max_y; i <= data->grid_min_y; i += vertical_spacing) {
        if (i >= data->grid_min_y && i <= data->grid_max_y) {
            cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
            cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
        }
    }

    // Disegna linee verticali
    for (int i = data->grid_min_x; i <= data->grid_max_x; i += horizontal_spacing) {
        if (i >= data->grid_min_x && i <= data->grid_max_x) {
            cairo_move_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->pan_y));
            cairo_line_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->pan_y));
        }
    }

    // griglia colore grigio
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 0.5);

    // Disegna linee orizzontali
    for (int i = data->grid_max_y; i <= data->grid_min_y; i+=1) {
        cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
        cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
    }

    // Disegna linee verticali
    for (int i = data->grid_min_x; i <= data->grid_max_x; i+=1) {
        cairo_move_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->pan_y));
        cairo_line_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->pan_y));
    }
    cairo_stroke(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);

    // Disegna il valore della coordinata verticale
    for (int i = data->grid_max_y; i <= data->grid_min_y; i++) {
        double y = (i * data->scale_y) + data->pan_y;

        cairo_set_source_rgba(cr, 0.2, 0.8, 0.2, 0.75); // verde
        cairo_move_to(cr, 10, FLIP_VERTICAL(y+5));
        cairo_show_text(cr, g_strdup_printf("%d", i));
    }

    int align = ALIGN_MIDDLE;

    // Disegna il valore della coordinata orizzontale
    for (int i = data->grid_min_x; i <= data->grid_max_x; i++) {
        double x = (i * data->scale_x) + data->pan_x;

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
#endif

    // asse orizzontale rosso
    cairo_set_source_rgb(cr, 0.8, 0.0, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((0 * data->scale_y) + data->pan_y));
    cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((0 * data->scale_y) + data->pan_y));
    cairo_stroke(cr);

    // asse verticale verde
    cairo_set_source_rgb(cr, 0.0, 0.8, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, (0 * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->pan_y));
    cairo_line_to(cr, (0 * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->pan_y));
    cairo_stroke(cr);
}

// Funzione per disegnare la griglia dinamica
void draw_dynamic_grid(cairo_t *cr, AppData *data)
{
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 2.0);

    // Calcola la densità delle linee in base alla scala
    double grid_density_x = 1.0 / data->scale_x; // Densità delle linee verticali
    double grid_density_y = 1.0 / data->scale_y; // Densità delle linee orizzontali

    // Calcola gli intervalli per le linee della griglia
    double vertical_spacing = fmax(1, (int)(grid_density_x * 10)); // Almeno 1 unità di distanza
    double horizontal_spacing = fmax(1, (int)(grid_density_y * 10)); // Almeno 1 unità di distanza

    // Disegna linee orizzontali
    for (int i = data->grid_min_y; i <= data->grid_max_y; i += horizontal_spacing) {
        cairo_move_to(cr, (data->grid_min_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
        cairo_line_to(cr, (data->grid_max_x * data->scale_x) + data->pan_x, FLIP_VERTICAL((i * data->scale_y) + data->pan_y));
    }

    // Disegna linee verticali
    for (int i = data->grid_min_x; i <= data->grid_max_x; i += vertical_spacing) {
        cairo_move_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_min_y * data->scale_y) + data->pan_y));
        cairo_line_to(cr, (i * data->scale_x) + data->pan_x, FLIP_VERTICAL((data->grid_max_y * data->scale_y) + data->pan_y));
    }

    cairo_stroke(cr);
}

// Funzione per disegnare il grafico
static void draw_graph(cairo_t *cr, AppData *data)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 1); // Colore blu

    telemetryData *td = data->tel->data;

    idxRange lapIdxRange = {};
    lapIdxRange = telemetry_getLapIdxRange(data->tel, data->spin_lap_val);
//lapIdxRange.idxFirst;
//lapIdxRange.idxLast;
//lapIdxRange.numIdxs;

#if 0
    float td0time = td[lapIdxRange.idxFirst].time;
    float td0distance = td[lapIdxRange.idxFirst].distance;
    if (td0time > 0 &&
        td0time < data->drawing_area_width &&
        td0distance > 0 &&
        td0distance < data->drawing_area_height)
    {
        cairo_move_to(cr,
                (td0time * data->scale_x) + data->pan_x,
                FLIP_VERTICAL((td0distance * data->scale_y) + data->pan_y));
    }
#endif

    for (size_t i = lapIdxRange.idxFirst; i < lapIdxRange.idxLast; i++) {
        float tdItime = td[i].time;
        float tdIdistance = td[i].distance;
        if (tdItime     * data->scale_x + data->pan_x > 0 &&
            tdItime     * data->scale_x + data->pan_x < data->drawing_area_width &&
            tdIdistance * data->scale_y + data->pan_y > 0 &&
            tdIdistance * data->scale_y + data->pan_y < data->drawing_area_height)
        {
            cairo_line_to(cr,
                    tdItime * data->scale_x + data->pan_x,
                    FLIP_VERTICAL(tdIdistance * data->scale_y + data->pan_y));
        }
    }
#if 0
    float td0time = td[1].time;
    float td0distance = td[1].distance;
    if (td0time > 0 &&
        td0time < data->drawing_area_width &&
        td0distance > 0 &&
        td0distance < data->drawing_area_height)
    {
        cairo_move_to(cr,
                (td0time * data->scale_x) + data->pan_x,
                FLIP_VERTICAL((td0distance * data->scale_y) + data->pan_y));
    }

    for (size_t i = 2; i < data->tel->num_samples; i++) {
        float tdItime = td[i].time;
        float tdIdistance = td[i].distance;
        if (tdItime     * data->scale_x + data->pan_x > 0 &&
            tdItime     * data->scale_x + data->pan_x < data->drawing_area_width &&
            tdIdistance * data->scale_y + data->pan_y > 0 &&
            tdIdistance * data->scale_y + data->pan_y < data->drawing_area_height)
        {
            cairo_line_to(cr,
                    tdItime * data->scale_x + data->pan_x,
                    FLIP_VERTICAL(tdIdistance * data->scale_y + data->pan_y));
        }
    }
#endif
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
//    draw_dynamic_grid(cr, data);

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

    double x    = (data->mouse_pos_X - data->pan_x) / data->scale_x;
    double y    = (data->drawing_area_height - data->mouse_pos_Y - data->pan_y) / data->scale_y; // Inverti Y
    double zoom_x = data->scale_x;
    double zoom_y = data->scale_y;

    gtk_label_set_text (GTK_LABEL(data->widgets.label_coords),
            g_strdup_printf("time:%+.4f, distance:%+.4f", x, y));
    gtk_label_set_text (GTK_LABEL(data->widgets.label_zoom),
            g_strdup_printf("zoom x:%+.3f\nzoom y:%+.3f",
                zoom_x, zoom_y));

    return FALSE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il movimento del mouse
gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, AppData *data)
{
    if (data->panning && (event->state & GDK_BUTTON1_MASK)) {
        // Aggiorna l'offset per il pan
        data->pan_x = event->x - data->offset_click_X;
        data->pan_y = FLIP_VERTICAL(event->y) + FLIP_VERTICAL(data->offset_click_Y);
        gtk_widget_queue_draw(widget);
    }

    if (data->selecting && (event->state & GDK_BUTTON1_MASK)) {
        data->select_end_x = event->x;
        data->select_end_y = event->y;
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }

    data->mouse_pos_X = event->x;
    data->mouse_pos_Y = event->y;

    double x    = (event->x - data->pan_x) / data->scale_x;
    double y    = (data->drawing_area_height - event->y - data->pan_y) / data->scale_y; // Inverti Y
    double zoom_x = data->scale_x;
    double zoom_y = data->scale_y;

    gtk_label_set_text (GTK_LABEL(data->widgets.label_coords),
            g_strdup_printf("time:%+.4f, distance:%+.4f", x, y));
    gtk_label_set_text (GTK_LABEL(data->widgets.label_zoom),
            g_strdup_printf("zoom x:%+.3f\nzoom y:%+.3f",
                zoom_x, zoom_y));

    return FALSE;
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

        data->select_start_x = (data->scale_x - data->mouse_pos_X + data->pan_x) / data->scale_x;
        data->select_start_y = (data->scale_y - data->mouse_pos_Y + data->pan_y) / data->scale_y;
        data->select_start_x = event->x;
        data->select_start_y = event->y;
        data->select_end_x = event->x;
        data->select_end_y = event->y;

        gtk_widget_queue_draw(widget);
    }
    // Inizia il pan
    data->offset_click_X = event->x - data->pan_x;
    data->offset_click_Y = FLIP_VERTICAL(event->y) + FLIP_VERTICAL(data->pan_y);

    return FALSE;
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

    return FALSE;
}

// Funzione per gestire il pan
gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        data->space_pressed = TRUE;
    }
    if (event->keyval == GDK_KEY_Control_L) {
        data->ctrl_pressed = TRUE;
    }
    if (event->keyval == GDK_KEY_Shift_L) {
        data->shift_pressed = TRUE;
    }

//    if (event->keyval == GDK_KEY_space) {
//        data->panning = TRUE;
//    }
//    if (event->keyval == GDK_KEY_Control_L) {
//        data->zooming = TRUE;
//    }
//    if (event->keyval == GDK_KEY_Shift_L) {
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

    return FALSE;
}

// Funzione per gestire il rilascio del pulsante del mouse
gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
{
    if (event->keyval == GDK_KEY_space) {
        data->space_pressed = FALSE;
    }
    if (event->keyval == GDK_KEY_Control_L) {
        data->ctrl_pressed = FALSE;
    }
    if (event->keyval == GDK_KEY_Shift_L) {
        data->shift_pressed = FALSE;
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
    return FALSE;
}

gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, AppData *data)
{
    double zoom_factor = 1.1; // Fattore di zoom
    double mouse_x = event->x;
    double mouse_y = event->y;

    // Calcola le coordinate della griglia prima dello zoom
    double grid_x = (mouse_x - data->pan_x) / data->scale_x;
    double grid_y = (data->drawing_area_height - mouse_y - data->pan_y) / data->scale_y; // Inverti Y

    if (event->direction == GDK_SCROLL_UP) {
        if (!data->shift_pressed && !data->ctrl_pressed) {
            data->scale_x *= zoom_factor;
            data->scale_y *= zoom_factor;
        }
        if (data->shift_pressed) {
            data->scale_x *= zoom_factor;
        }
        if (data->ctrl_pressed) {
            data->scale_y *= zoom_factor;
        }
    } else if (event->direction == GDK_SCROLL_DOWN) {
        if (!data->shift_pressed && !data->ctrl_pressed) {
            data->scale_x /= zoom_factor;
            data->scale_y /= zoom_factor;
        }
        if (data->shift_pressed) {
            data->scale_x /= zoom_factor;
        }
        if (data->ctrl_pressed) {
            data->scale_y /= zoom_factor;
        }
    }

    // Calcola il nuovo offset in modo che il punto sotto il mouse rimanga fisso
    data->pan_x = mouse_x - grid_x * data->scale_x;
    data->pan_y = (data->drawing_area_height - mouse_y) - grid_y * data->scale_y;

    gtk_widget_queue_draw(widget); // Richiesta di ridisegno

    return FALSE;
}

gboolean on_window_check_resize(GtkContainer *window, AppData *data)
{
    gint win_width;
    gint win_height;
    gtk_window_get_size(GTK_WINDOW(window), &win_width, &win_height);

    if (data) {
        gtk_widget_set_size_request(GTK_WIDGET(data->widgets.drawing_area), win_width, win_height);

        GdkRectangle allocation;
        gtk_widget_get_allocation(GTK_WIDGET(data->widgets.label_zoom), &allocation);

        gint pos_x = win_width - allocation.width - 30;
        gint pos_y = 0;

        gtk_fixed_move(GTK_FIXED(data->widgets.fixed), data->widgets.label_zoom, pos_x, pos_y);
    }

    return FALSE;
}

gboolean on_spin_lap_value_changed(GtkWidget *widget, GdkEvent *event, AppData *data)
{
    data->spin_lap_val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(data->widgets.spin_lap));
    gtk_widget_queue_draw(data->widgets.drawing_area); // Richiesta di ridisegno

    int num_laps = arrlen(data->tel->lapIndex) - 1;
    gtk_adjustment_set_lower(data->widgets.adjustment_spin_lap, 0);
    gtk_adjustment_set_upper(data->widgets.adjustment_spin_lap, num_laps);

    idxRange idxR = telemetry_getLapIdxRange(data->tel, data->spin_lap_val);

//    idxR.idxFirst;
//    idxR.idxLast;
//    idxR.numIdxs;

//    size_t pan_x = (data->tel->data[(size_t)(idxR.idxFirst + idxR.numIdxs/2)].time);
//    size_t pan_y = (data->tel->data[(size_t)(idxR.idxFirst + idxR.numIdxs/2)].distance);
//    data->pan_x = pan_x + data->drawing_area_width/2.0;
//    data->pan_y = pan_y + FLIP_VERTICAL(data->drawing_area_width/2);

    return FALSE;
}

gboolean on_file_chooser_button_file_set(GtkWidget *widget, AppData *data)
{
    char *filename;
    GtkTreeIter iter;

    // Ottieni il nome del file selezionato
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));

    // Aggiungi il nome del file e i dati associati al modello
    gtk_list_store_append(GTK_LIST_STORE(data->widgets.list_store_cars), &iter);
    gtk_list_store_set(GTK_LIST_STORE(data->widgets.list_store_cars), &iter, 0, filename, 1, 0, -1); // 0 per il nome del file, 1 per i dati associati

    // Libera la memoria del nome del file
    g_free(filename);
    return TRUE;
}

gboolean on_button_remove_clicked(GtkWidget *widget, AppData *data)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gboolean valid;
printf("1\n");

    // Ottieni l'indice attivo
    int active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(data->widgets.combo_box_cars));
printf("2\n");

    if (active_index != -1) {
        // Ottieni il modello del combo box
        model = gtk_combo_box_get_model(GTK_COMBO_BOX(data->widgets.combo_box_cars));

        // Ottieni l'iter della riga attiva
        valid = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->widgets.combo_box_cars), &iter);
        if (valid) {
            // Rimuovi l'elemento dal list store
            gtk_list_store_remove(data->widgets.list_store_cars, &iter);
        }
    }

    return TRUE;
}
