#ifndef TELEMETRY_H_
#define TELEMETRY_H_

// interfaces/raceman.h:56:#define RCM_MAX_DT_SIMU     0.002
#define RCM_MAX_DT_SIMU 0.002
#define MAX_LINE_SIZE   1024

typedef struct
{
    size_t idxFirst;
    size_t idxLast;
} idxRange;

FILE* openFile(char *fname);
void closeFile(FILE *f);

typedef struct
{
    float time;        // Tempo in secondi
    float distance;    // Distanza in metri
} telemetryData;
typedef struct
{
    telemetryData  *data;       // Array dinamico di punti dati
    size_t         *lapIndex;   // Array dinamico per gli indici di inizio giro
    float           lap0Time;
    float           lapAvgSpeed;
    float           raceAvgSpeed;
} telemetry;

typedef struct
{
    float   distanza;           // Distanza trovata
    size_t  indicePrecedente;   // Indice del punto precedente
    size_t  indiceSuccessivo;   // Indice del punto successivo
} telemetry_binSearchResult;

telemetry* telemetry_ctor();
void      telemetry_dtor(telemetry *tel);
void telemetry_addDataPoint(telemetry *tel, float time, float distance);
void telemetry_addLapIndex(telemetry *tel, size_t index);
telemetry_binSearchResult telemetry_distanceAtTime(telemetry *tel, float tempoRicercato);
int telemetry_ricercaBinaria(telemetry *tel, float tempoRicercato, size_t *indicePrecedente, size_t *indiceSuccessivo);
void telemetry_loadFromCSV(telemetry *tel, FILE *f);
idxRange telemetry_getLapIdxRange(telemetry *tel, int lapN);
void telemetry_dumpLap(telemetry *tel, int lapN);

#endif /* TELEMETRY_H_ */
