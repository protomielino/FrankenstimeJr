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


telemetry* telemetry_ctor()
{
    telemetry *tel = calloc(1, sizeof(telemetry));

    arrpush(tel->data, ((telemetryData){ 0.0, 0.0 }));
    arrpush(tel->lapIndex, 0);

    return tel;
}

void telemetry_dtor(telemetry *tel)
{
    if (arrlen(tel->data) > 0)
    {
        arrfree(tel->data);
        tel->data = NULL;
    }
    if (arrlen(tel->lapIndex) > 0)
    {
        arrfree(tel->lapIndex);
        tel->lapIndex = NULL;
    }
    
    free(tel);
    tel = NULL;
}

void telemetry_addDataPoint(telemetry *tel, float time, float distance)
{
    telemetryData newPoint = { time, distance };
    arrpush(tel->data, newPoint); // Aggiunge il nuovo punto all'array dinamico
}

void telemetry_addLapIndex(telemetry *tel, size_t index)
{
    arrpush(tel->lapIndex, index); // Aggiunge l'indice di inizio giro all'array dinamico
}

int telemetry_ricercaBinaria(telemetry *tel, float tempoRicercato, size_t *indicePrecedente, size_t *indiceSuccessivo)
{
    int inizio = 0;
    int fine = arrlen(tel->data) - 1;

    while (inizio <= fine) {
        int medio = inizio + (fine - inizio) / 2;

        if (tel->data[medio].time == tempoRicercato) {
            *indicePrecedente = (medio > 0) ? medio - 1 : -1; // Indice precedente
            *indiceSuccessivo = (medio < arrlen(tel->data) - 1) ? medio + 1 : -1; // Indice successivo
            return medio; // Restituisce l'indice trovato
        }

        if (tel->data[medio].time > tempoRicercato) {
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

telemetry_binSearchResult telemetry_distanceAtTime(telemetry *tel, float tempoRicercato)
{
    telemetry_binSearchResult risultato = {0, -1, -1}; // Inizializza il risultato

    if (arrlen(tel->data) == 0) {
        printf("Nessun dato disponibile.\n");
        return risultato; // Restituisce il risultato vuoto
    }

    size_t indicePrecedente = -1;
    size_t indiceSuccessivo = -1;
    size_t indiceTrovato = telemetry_ricercaBinaria(tel, tempoRicercato, &indicePrecedente, &indiceSuccessivo);

    if (indiceTrovato != -1) {
        risultato.distanza = tel->data[indiceTrovato].distance; // Distanza trovata
    } else {
        // Se non è stato trovato il tempo esatto, restituisci la distanza del punto più vicino
        if (indicePrecedente >= 0) {
            risultato.distanza = tel->data[indicePrecedente].distance; // Distanza del punto più vicino a sinistra
        }
        if (indiceSuccessivo < arrlen(tel->data)) {
            // Confronta con il punto più vicino a destra
            if (fabs(tel->data[indiceSuccessivo].time - tempoRicercato) < fabs(tel->data[indicePrecedente].time - tempoRicercato)) {
                risultato.distanza = tel->data[indiceSuccessivo].distance;
            }
        }
    }

    risultato.indicePrecedente = indicePrecedente;
    risultato.indiceSuccessivo = indiceSuccessivo;

    return risultato; // Restituisce il risultato
}

void telemetry_loadFromCSV(telemetry *tel, FILE *f)
{
    char line[MAX_LINE_SIZE];

    // Leggi il file riga per riga e salva
    while (fgets(line, sizeof(line), f)) {
        static int prevLap = 0;
        int   _laps = 0;
        float _currTime = 0.0f;
        float _distRaced = 0.0f;
        float _timeRaced = 0.0f;
        float _curLapTime = 0.0f;
        float _distFromStartLine = 0.0f;
        float _currLapTimeAtTrackPosition_corrected = 0.0f;

        sscanf(line, "%d, %f, %f, %f, %f, %f",
                &_laps,
                &_currTime,
                &_distRaced,
                &_curLapTime,
                &_distFromStartLine,
                &_currLapTimeAtTrackPosition_corrected);

        // registra l'indice iniziale del nuovo giro
        if (prevLap < _laps) {
            telemetry_addLapIndex(tel, arrlen(tel->data));

            // registra il tempo dal "GO" all'inizio del primo giro
            if (prevLap == 0) {
                tel->lap0Time = _currTime - RCM_MAX_DT_SIMU;
            }

            prevLap = _laps;
        }

        _timeRaced = _curLapTime + _currTime - tel->lap0Time;; // - tel->lap0Time;

        // registra i dati della simulazione
        telemetry_addDataPoint(tel, _timeRaced, _distRaced);

//        printf("%f, %f\n", _timeRaced, _distRaced);
//        printf("%f, %f\n", tel->data[arrlen(tel->data) - 1].time, tel->data[arrlen(tel->data) - 1].distance);

//        printf("%d, %f, %f, %f, %f, %f, %f, %f\n",
//                _laps,
//                tel->lap0Time,
//                _currTime,
//                _timeRaced,
//                _distRaced,
//                _curLapTime,
//                _distFromStartLine,
//                _currLapTimeAtTrackPosition_corrected);
    }
}

idxRange telemetry_getLapIdxRange(telemetry *tel, int lapN)
{
    idxRange ret = {};

    if (lapN < arrlen(tel->lapIndex) - 1) {
        size_t currLapIdx = tel->lapIndex[lapN];
        size_t nextLapIdx = tel->lapIndex[lapN + 1];

        ret.idxFirst = currLapIdx;
        ret.idxLast  = nextLapIdx - 1;
    } else {
        size_t currLapIdx = tel->lapIndex[lapN];
        size_t lastDataIdx = arrlen(tel->data);

        ret.idxFirst = currLapIdx;
        ret.idxLast  = lastDataIdx;
    }
    
    ret.numIdxs =  ret.idxLast - ret.idxFirst;

    if (lapN == 0) {
        ret.numIdxs += 1;
    }

    return ret;
}

void telemetry_dumpLap(telemetry *tel, int lapN)
{
    idxRange range = {};
    range = telemetry_getLapIdxRange(tel, lapN);

    for (size_t idx = range.idxFirst; idx <= range.idxLast; idx++) {
        printf("%ld, %f, %f\n", idx, tel->data[idx].time, tel->data[idx].distance);
    }
}
