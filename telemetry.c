#include <stdlib.h>
#include <stdio.h>
#include <math.h> // Per la funzione fabs

#include "telemetry.h"

#include "stb_ds.h"

FILE* openFile(char *fname)
{
    FILE *f = fopen(fname, "r");

    if (!f) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    return f;
}

void closeFile(FILE *f)
{
    fclose(f);
}


telemetry telemetry_ctor()
{
    telemetry tel = {};
    arrpush(tel.data, ((telemetryData){ 0.0, 0.0 }));

    return tel;
}

void telemetry_dtor(telemetry tel)
{
    if (arrlen(tel.data) > 0)
    {
        arrfree(tel.data);
    }
    if (arrlen(tel.lapIndex) > 0)
    {
        arrfree(tel.lapIndex);
    }
}

void telemetry_addDataPoint(telemetry tel, float time, float distance)
{
    telemetryData newPoint = { time, distance };
    arrpush(tel.data, newPoint); // Aggiunge il nuovo punto all'array dinamico
}

void telemetry_addLapIndex(telemetry tel, int index)
{
    arrpush(tel.lapIndex, index); // Aggiunge l'indice di inizio giro all'array dinamico
}

int telemetry_ricercaBinaria(telemetry tel, float tempoRicercato, int *indicePrecedente, int *indiceSuccessivo)
{
    int inizio = 0;
    int fine = arrlen(tel.data) - 1;

    while (inizio <= fine) {
        int medio = inizio + (fine - inizio) / 2;

        if (tel.data[medio].time == tempoRicercato) {
            *indicePrecedente = (medio > 0) ? medio - 1 : -1; // Indice precedente
            *indiceSuccessivo = (medio < arrlen(tel.data) - 1) ? medio + 1 : -1; // Indice successivo
            return medio; // Restituisce l'indice trovato
        }

        if (tel.data[medio].time > tempoRicercato) {
            fine = medio - 1;
        } else {
            inizio = medio + 1;
        }
    }

    // Se non è stato trovato il tempo esatto, restituisci gli indici
    *indicePrecedente = fine; // Indice del punto più vicino a sinistra
    *indiceSuccessivo = inizio; // Indice del punto più vicino a destra

    return -1; // Indica che non è stato trovato il tempo esatto
}

telemetry_risultatoDistanza telemetry_distanceAtTime(telemetry tel, float tempoRicercato)
{
    telemetry_risultatoDistanza risultato = {0, -1, -1}; // Inizializza il risultato

    if (arrlen(tel.data) == 0) {
        printf("Nessun dato disponibile.\n");
        return risultato; // Restituisce il risultato vuoto
    }

    int indicePrecedente = -1;
    int indiceSuccessivo = -1;
    int indiceTrovato = telemetry_ricercaBinaria(tel, tempoRicercato, &indicePrecedente, &indiceSuccessivo);

    if (indiceTrovato != -1) {
        risultato.distanza = tel.data[indiceTrovato].distance; // Distanza trovata
    } else {
        // Se non è stato trovato il tempo esatto, restituisci la distanza del punto più vicino
        if (indicePrecedente >= 0) {
            risultato.distanza = tel.data[indicePrecedente].distance; // Distanza del punto più vicino a sinistra
        }
        if (indiceSuccessivo < arrlen(tel.data)) {
            // Confronta con il punto più vicino a destra
            if (fabs(tel.data[indiceSuccessivo].time - tempoRicercato) < fabs(tel.data[indicePrecedente].time - tempoRicercato)) {
                risultato.distanza = tel.data[indiceSuccessivo].distance;
            }
        }
    }

    risultato.indicePrecedente = indicePrecedente;
    risultato.indiceSuccessivo = indiceSuccessivo;

    return risultato; // Restituisce il risultato
}

void telemetry_loadFromCSV(telemetry tel, FILE *f)
{
    char line[MAX_LINE_SIZE];
    size_t idx = 0;

    // Leggi il file riga per riga e salva
    while (fgets(line, sizeof(line), f)) {
        static int prevLap = 0;
        int   _laps = 0;
        float _curTime;
        float _distRaced = 0.0f;
        float _curLapTime = 0.0f;
        float _distFromStartLine = 0.0f;
        float _currLapTimeAtTrackPosition_corrected = 0.0f;

        sscanf(line, "%d, %f, %f, %f, %f, %f",
                &_laps,
                &_curTime,
                &_distRaced,
                &_curLapTime,
                &_distFromStartLine,
                &_currLapTimeAtTrackPosition_corrected);

        printf("%d, %f, %f, %f, %f, %f\n",
                _laps,
                _curTime,
                _distRaced,
                _curLapTime,
                _distFromStartLine,
                _currLapTimeAtTrackPosition_corrected);
//        telemetry_addDataPoint(tel, time, dist);

        // registra il tempo dal "GO" all'inizio del primo giro
        if (_laps == 0) {
//            tel.lap0Time = time;
        }

        // registra il l'indice iniziale del nuovo giro
        if (prevLap < _laps) {
            telemetry_addLapIndex(tel, arrlen(tel.data) - 1);
            prevLap = _laps;
        }

        idx++;
    }
}
