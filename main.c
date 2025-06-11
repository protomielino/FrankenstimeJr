#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <stdbool.h>

#include "telemetry.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH   640
#define HEIGHT  400
#define PAN

#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
#define ALIGN_MIDDLE 3

typedef struct
{
    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkWidget *label;

} AppWidgets;

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
    gboolean space_pressed;

    int drawing_area_width;
    int drawing_area_height;

    AppWidgets widgets;

    telemetry *tel;
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

    telemetryData *td = data->tel->data;

    float td0time = td[0].time;
    float td0distance = (td[0].distance/50);
    if (td0time > 0 && td0time < data->drawing_area_width &&
        td0distance > 0 && td[0].distance < data->drawing_area_height) {
        // Inizia a disegnare il grafico
        cairo_move_to(cr, td0time * data->scale + data->offset_x, td0distance * data->scale + data->offset_y);
    }

    for (size_t i = 1; i < data->tel->num_samples; i++) {
        float tdItime = td[i].time;
        float tdIdistance = (td[i].distance/50);
        if (tdItime * data->scale + data->offset_x> 0 && tdItime * data->scale + data->offset_x< data->drawing_area_width &&
            tdIdistance  * data->scale + data->offset_y> 0 && td[i].distance  * data->scale + data->offset_y< data->drawing_area_height) {
            cairo_line_to(cr, tdItime * data->scale + data->offset_x, tdIdistance * data->scale + data->offset_y);
        }
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

    cairo_set_source_rgb(cr, 1, 1, 1);

//    cairo_move_to(cr, 10, 30);
//    cairo_show_text(cr,
//            (data->selecting ?
//                    "selecting" :
//                    data->panning ?
//                            "panning" :
//                            "")); // Scrivi il valore

    return TRUE; // Restituisce FALSE per permettere ulteriori elaborazioni
}

// Funzione per gestire il movimento del mouse
static gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, AppData *data)
{
    if (data->panning && (event->state & GDK_BUTTON1_MASK)) {
        // Aggiorna l'offset per il pan
        data->offset_x = event->x - data->offset_click_X;
        data->offset_y = event->y - data->offset_click_Y;
        gtk_widget_queue_draw(widget);
    }

    if (data->selecting && (event->state & GDK_BUTTON1_MASK)) {
        data->select_end_x = event->x;
        data->select_end_y = event->y;
        gtk_widget_queue_draw(widget); // Ridisegna il widget
    }

    data->mouse_pos_X = event->x;
    data->mouse_pos_Y = event->y;

    gtk_label_set_text (GTK_LABEL(data->widgets.label),
            g_strdup_printf("x:%+.3f\ty:%+.3f",
                        (data->select_start_x * data->scale - data->offset_x + data->mouse_pos_X) / data->scale,
                        (data->select_start_y * data->scale - data->offset_y + data->mouse_pos_Y) / data->scale));


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

        data->select_start_x = (data->scale - data->mouse_pos_X + data->offset_x) / data->scale;
        data->select_start_y = (data->scale - data->mouse_pos_Y + data->offset_y) / data->scale;
        data->select_start_x = event->x;
        data->select_start_y = event->y;
        data->select_end_x = event->x;
        data->select_end_y = event->y;

        gtk_widget_queue_draw(widget);
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
    }    gtk_widget_set_hexpand(data->widgets.drawing_area, TRUE);
    return TRUE;
}

// Funzione per gestire il pan
static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
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
static gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, AppData *data)
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
    if (argc < 4) {
        char buf[1024] = {};
        snprintf(buf, sizeof(buf), "Uso: %s <nome file csv> <numero lap> <stampa lap>", argv[0]);
        perror(buf);
        exit(EXIT_FAILURE);
    }

    char *fileName    = argv[1];
    int   lapNumber   = atoi(argv[2]);
    bool  dumpLapData = atoi(argv[3]) == 0 ? false : true;

    FILE *f = openFile(fileName);

    telemetry *tel = telemetry_ctor();

    telemetry_loadFromCSV(tel, f);


    if (lapNumber >= 0 && lapNumber < arrlen(tel->lapIndex)) {
        if (dumpLapData) {
            telemetry_dumpLap(tel, lapNumber);
        }

        idxRange lapIdxRange = {};
        lapIdxRange = telemetry_getLapIdxRange(tel, lapNumber);
        printf("lap: %d, first idx: %ld, last idx: %ld\n", lapNumber, lapIdxRange.idxFirst, lapIdxRange.idxLast);
        printf("lap: %d, num samples: %ld\n", lapNumber, lapIdxRange.numIdxs);
    } else {
        printf("<numero lap> deve essere fra 0 e %ld\n", arrlen(tel->lapIndex) - 1);
    }
    printf("arrlen(tel->data): %ld \t %f[Mb]\n", arrlen(tel->data), ((arrlen(tel->data) * sizeof(telemetryData) / 1024.0) / 1024.0));
    printf("arrcap(tel->data): %ld \t %f[Mb]\n", arrcap(tel->data), ((arrcap(tel->data) * sizeof(telemetryData) / 1024.0) / 1024.0));
    printf("tot num laps: %ld\n", arrlen(tel->lapIndex)); // including lap 0 and cool down laps

//    // Esempio di ricerca della distanza a un dato tempo
//    for (float s = 119.0; s < 120.0; s+=0.002) {
//        float tempoRicercato = s; // Cambia questo valore per testare
//        telemetry_risultatoDistanza risultato = telemetry_distanceAtTime(tel, tempoRicercato);
//
//        printf("\n");
//
//        if (risultato.indicePrecedente != -1) {
//            printf("Distanza a %.3f[s]: %f[m] (Indice: %ld)\n", tempoRicercato, risultato.distanza, risultato.indicePrecedente);
//        } else {
//            printf("Distanza non trovata per il tempo specificato.\n");
//        }
//
//        // Stampa gli indici precedenti e successivi
//        if (risultato.indicePrecedente != -1) {
//            printf("Indice precedente: %ld, Distanza: %f[m]\n", risultato.indicePrecedente, tel->data[risultato.indicePrecedente].distance);
//        }
//        if (risultato.indiceSuccessivo != -1) {
//            printf("Indice successivo: %ld, Distanza: %f[m]\n", risultato.indiceSuccessivo, tel->data[risultato.indiceSuccessivo].distance);
//        }
//    }

#if 0
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Grafico con Cairo");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), tel);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), tel);

    gtk_widget_show_all(window);
    gtk_main();
#endif

    closeFile(f);
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
    AppData data = {}; // Inizializza i dati
    data.tel = tel;
    data.drawing_area_width = WIDTH;
    data.drawing_area_height = HEIGHT;

    gtk_init(&argc, &argv);

    // Carica l'interfaccia da Glade
    GtkBuilder *builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "graph.glade", NULL) == 0) {
        g_printerr("Error loading file: test1.glade\n");
        return 1; // o gestisci l'errore come preferisci
    }

    data.widgets.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_title(GTK_WINDOW(data.widgets.window), "Grafico Cairo");
    gtk_window_set_default_size(GTK_WINDOW(data.widgets.window), data.drawing_area_width, data.drawing_area_height);
    g_signal_connect(data.widgets.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(data.widgets.window), "key-press-event", G_CALLBACK(on_key_press_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.window), "key-release-event", G_CALLBACK(on_key_release_event), &data);

    gtk_builder_connect_signals(builder, NULL);

    data.scale = 15.0;
    data.offset_x = data.drawing_area_width/2.0;
    data.offset_y = data.drawing_area_height/2.0; // Inizializza i dati

    data.widgets.drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "draw", G_CALLBACK(on_draw_event), &data);

    data.widgets.label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));

    // Aggiungi gestori per il pan, lo zoom e la selezione
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "motion-notify-event", G_CALLBACK(on_motion_notify_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "button-press-event", G_CALLBACK(on_button_press_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "button-release-event", G_CALLBACK(on_button_release_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "scroll-event", G_CALLBACK(on_scroll_event), &data);

    // Abilita gli eventi del mouse
    gtk_widget_set_events(data.widgets.drawing_area, gtk_widget_get_events(data.widgets.drawing_area) |
            GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK);

    gtk_widget_show_all(data.widgets.window);
    gtk_main();

    telemetry_dtor(tel);

    return 0;
}
