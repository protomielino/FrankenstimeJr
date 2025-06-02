#include <stdlib.h>
#include <stdio.h>
#include <math.h> // Per la funzione fabs
#include <stdbool.h>

#include "telemetry.h"
#include "grafico_cairo.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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
    printf("toto num laps: %ld\n", arrlen(tel->lapIndex)); // including lap 0 and cool down laps

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

    // Libera la memoria
    telemetry_dtor(tel);

    closeFile(f);

    return 0;
}

// data is from speed dreams simulation in form of:
//fprintf(car->_csv_log, "%d, %f, %f, %f, %f\n",
//            car->_laps,
//            car->_curLapTime,
//            car->_distFromStartLine,
//            car->_distRaced,
//            car->_currLapTimeAtTrackPosition_corrected);
