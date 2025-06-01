#include <stdlib.h>
#include <stdio.h>
#include <math.h> // Per la funzione fabs

#include "telemetry.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"


int main()
{
    FILE *f = openFile("telemetry_1.csv");

    telemetry tel = telemetry_ctor();

    telemetry_loadFromCSV(tel, f);


//    // Aggiungi alcuni punti dati
//    for (float t = 0; t <= 120.0; t += 0.100) {
//        float distanza = t * 100; // Esempio: distanza = velocità * tempo
//        aggiungiPuntoDati(t, distanza);
//
//        // Aggiungi indici di giro
//        if ((int)(distanza) % 1000 == 0) {
//            aggiungiIndiceGiro(arrlen(dati) - 1);
//        }
//    }

    // Esempio di ricerca della distanza a un dato tempo
    for (float s = 119.0; s < 120.0; s+=0.002) {
        float tempoRicercato = s; // Cambia questo valore per testare
        telemetry_risultatoDistanza risultato = telemetry_distanceAtTime(tel, tempoRicercato);

        printf("\n");

        if (risultato.indicePrecedente != -1) {
            printf("Distanza a %.3f[s]: %f[m] (Indice: %d)\n", tempoRicercato, risultato.distanza, risultato.indicePrecedente);
        } else {
            printf("Distanza non trovata per il tempo specificato.\n");
        }

        // Stampa gli indici precedenti e successivi
        if (risultato.indicePrecedente != -1) {
            printf("Indice precedente: %d, Distanza: %f[m]\n", risultato.indicePrecedente, tel.data[risultato.indicePrecedente].distance);
        }
        if (risultato.indiceSuccessivo != -1) {
            printf("Indice successivo: %d, Distanza: %f[m]\n", risultato.indiceSuccessivo, tel.data[risultato.indiceSuccessivo].distance);
        }
    }

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
