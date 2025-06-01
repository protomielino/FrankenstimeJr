#ifndef TELEMETRY_H_
#define TELEMETRY_H_

// interfaces/raceman.h:56:#define RCM_MAX_DT_SIMU     0.002
#define RCM_MAX_DT_SIMU 0.002
#define MAX_LINE_SIZE   1024

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
    int            *lapIndex;   // Array dinamico per gli indici di inizio giro
    float           lap0Time;
    float           lapAvgSpeed;
    float           raceAvgSpeed;
} telemetry;

typedef struct
{
    float   distanza;           // Distanza trovata
    int     indicePrecedente;   // Indice del punto precedente
    int     indiceSuccessivo;   // Indice del punto successivo
} telemetry_risultatoDistanza;

telemetry telemetry_ctor();
void      telemetry_dtor(telemetry tel);
void telemetry_addDataPoint(telemetry tel, float time, float distance);
void telemetry_addLapIndex(telemetry tel, int index);
telemetry_risultatoDistanza telemetry_distanceAtTime(telemetry tel, float tempoRicercato);
int telemetry_ricercaBinaria(telemetry tel, float tempoRicercato, int *indicePrecedente, int *indiceSuccessivo);
void telemetry_loadFromCSV(telemetry tel, FILE *f);

#endif /* TELEMETRY_H_ */
