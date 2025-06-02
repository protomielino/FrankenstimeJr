#include <stdlib.h>
#include <stdio.h>
#include <math.h> // Per la funzione fabs
#include <stdbool.h>

#include "telemetry.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int main(int argc, char *argv[])
{
    char *fileName    = argv[1];
    int   lapNumber   = atoi(argv[2]);
    bool  dumpLapData = atoi(argv[3]) == 0 ? false : true;

    FILE *f = openFile(fileName);

    telemetry *tel = telemetry_ctor();

    telemetry_loadFromCSV(tel, f);


//    for (size_t i = 0; i < arrlen(tel->lapIndex); i++) {
//        printf("%ld , %ld, %f, %f\n", i, tel->lapIndex[i] - 1, tel->data[i + tel->lapIndex[i]].time, tel->data[i + tel->lapIndex[i]].distance);
//    }

    if (lapNumber >= 0 && lapNumber < arrlen(tel->lapIndex)) {
        if (dumpLapData) {
            telemetry_dumpLap(tel, lapNumber);
        }

        idxRange lapIdxRange = {};
        lapIdxRange = telemetry_getLapIdxRange(tel, lapNumber);
        printf("lap: %d, first idx: %ld, last idx: %ld\n", lapNumber, lapIdxRange.idxFirst, lapIdxRange.idxLast);
        printf("lap: %d, num samples: %ld\n", lapNumber, lapIdxRange.idxLast - lapIdxRange.idxFirst);
    } else {
        printf("argomento deve essere fra 0 e %ld\n", arrlen(tel->lapIndex) - 1);
    }
    printf("%ld\n", arrlen(tel->data));
    printf("%ld\n", arrlen(tel->lapIndex));

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

    // Libera la memoria
    telemetry_dtor(tel);

    closeFile(f);

    return 0;
}

//fprintf(car->_csv_log, "%d, %f, %f, %f, %f\n",
//            car->_laps,
//            car->_curLapTime,
//            car->_distFromStartLine,
//            car->_distRaced,
//            car->_currLapTimeAtTrackPosition_corrected);
