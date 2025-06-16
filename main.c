#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <stdbool.h>

#include "telemetry.h"
#include "graphics.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int main(int argc, char *argv[])
{
//----------------------------------------------------------------------------------------
// init telemetry
//----------------------------------------------------------------------------------------

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

    closeFile(f);

//----------------------------------------------------------------------------------------
// init GTK
//----------------------------------------------------------------------------------------

    AppData data = {}; // Inizializza i dati
    data.tel = tel;
    data.drawing_area_width = WIDTH;
    data.drawing_area_height = HEIGHT;
    data.scale_x = 10.0;
    data.scale_y = 10.0;
    data.pan_x = data.drawing_area_width/2.0;
    data.pan_y = data.drawing_area_height/2.0;
    int tel_num_samples = tel->num_samples;
    data.grid_min_x = tel->data[1].time;
    data.grid_max_x = tel->data[tel_num_samples-1].time;;
    data.grid_min_y = tel->data[1].distance;
    data.grid_max_y = tel->data[tel_num_samples-1].distance;;

    printf("%d\n", data.drawing_area_width);

    gtk_init(&argc, &argv);

    // Carica l'interfaccia da Glade
    GtkBuilder *builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "graph.glade", NULL) == 0) {
        g_printerr("Error loading file: test1.glade\n");
        return EXIT_FAILURE; // o gestisci l'errore come preferisci
    }

    // inizializza window e collega gli eventi associati
    data.widgets.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    gtk_builder_connect_signals(builder, NULL);

    data.widgets.fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
    data.widgets.drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    data.widgets.label_coords = GTK_WIDGET(gtk_builder_get_object(builder, "label_coords"));
    data.widgets.label_zoom = GTK_WIDGET(gtk_builder_get_object(builder, "label_zoom"));
    data.widgets.spin_lap = GTK_WIDGET(gtk_builder_get_object(builder, "spin_lap"));
    data.widgets.adjustment_spin_lap = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment_spin_lap"));
    data.widgets.file_chooser_button = GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser_button"));
    data.widgets.combo_box_cars = GTK_WIDGET(gtk_builder_get_object(builder, "combo_box_cars"));
    data.widgets.list_store_cars = GTK_LIST_STORE(gtk_builder_get_object(builder, "list_store_cars"));
    data.widgets.button_remove = GTK_WIDGET(gtk_builder_get_object(builder, "button_remove"));

    gtk_window_set_title(GTK_WINDOW(data.widgets.window), "Grafico Cairo");
    gtk_window_set_default_size(GTK_WINDOW(data.widgets.window), data.drawing_area_width, data.drawing_area_height);
    g_signal_connect(G_OBJECT(data.widgets.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(data.widgets.window), "key-press-event", G_CALLBACK(on_key_press_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.window), "key-release-event", G_CALLBACK(on_key_release_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.window), "check-resize", G_CALLBACK(on_window_check_resize), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "draw", G_CALLBACK(on_draw_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "motion-notify-event", G_CALLBACK(on_motion_notify_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "button-press-event", G_CALLBACK(on_button_press_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "button-release-event", G_CALLBACK(on_button_release_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.drawing_area), "scroll-event", G_CALLBACK(on_scroll_event), &data);
    g_signal_connect(G_OBJECT(data.widgets.spin_lap), "value-changed", G_CALLBACK(on_spin_lap_value_changed), &data);
    g_signal_connect(G_OBJECT(data.widgets.file_chooser_button), "file-set", G_CALLBACK(on_file_chooser_button_file_set), &data);
    g_signal_connect(G_OBJECT(data.widgets.button_remove), "clicked", G_CALLBACK(on_button_remove_clicked), &data);
    g_signal_connect(G_OBJECT(data.widgets.list_store_cars), "clicked", G_CALLBACK(on_button_remove_clicked), &data);

    gtk_combo_box_set_model(GTK_COMBO_BOX(data.widgets.combo_box_cars), GTK_TREE_MODEL(data.widgets.list_store_cars));

    gtk_widget_set_hexpand(data.widgets.drawing_area, TRUE);

    // Abilita gli eventi del mouse
    gtk_widget_set_events(
            data.widgets.drawing_area,
            gtk_widget_get_events(data.widgets.drawing_area)
                | GDK_BUTTON_PRESS_MASK
                | GDK_POINTER_MOTION_MASK
                | GDK_BUTTON_RELEASE_MASK
                | GDK_SCROLL_MASK);

    gtk_widget_show_all(data.widgets.window);
    gtk_main();

    telemetry_dtor(tel);

    return 0;
}
