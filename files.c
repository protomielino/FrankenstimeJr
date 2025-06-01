#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h" 


#define RCM_MAX_DT_SIMU 0.002

typedef struct
{
    float time;
    float dist;
} telemetryData;
typedef struct
{
    telemetryData   *data;
    size_t          *idxLap;
    int              bestLap;
    size_t           idxBestLap;
    float            lap0Time;
} telemetry;

float calcLapTime(telemetry tel, int lap)
{
    float ret = 0.0;

    if ( lap > 0 && arrlen(tel.idxLap) > (lap + 1) ) {
        size_t lapIdx = tel.idxLap[lap];
        size_t lapLastIdx = tel.idxLap[lap+1] - 1;
        ret = tel.data[lapLastIdx].time - tel.data[lapIdx].time;
    }

    return ret;
}

int main()
{
    FILE *file = fopen("csv_log_car_0.csv", "r");
    if (!file) {
        perror("Unable to open file");
        return 1;
    }

    telemetry tel = {};

//    lap0
    arrput(tel.idxLap, 0);

    char line[1024];
    // Leggi il file riga per riga e salva
    while (fgets(line, sizeof(line), file)) {
        static int prevLap = 0;
        int _laps = 0;
        float _curLapTime;
        float _distFromStartLine = 0.0f;
        float _distRaced = 0.0f;
        float _currLapTimeAtTrackPosition_corrected = 0.0f;

        sscanf(line, "%d, %f, %f, %f, %f",
                &_laps,
                &_curLapTime,
                &_distFromStartLine,
                &_distRaced,
                &_currLapTimeAtTrackPosition_corrected);

        // interfaces/raceman.h:56:#define RCM_MAX_DT_SIMU     0.002
        float totTime = (float)idx * RCM_MAX_DT_SIMU;
        printf("%ld, %f, %d, %ld, %f, %f\n", idx, totTime, _laps, _curLapTime, _distFromStartLine, _distRaced, _currLapTimeAtTrackPosition_corrected);

        if (_laps == 0) {
            tel.lap0Time = totTime;
        } else {
            telemetryData td = { totTime, _distRaced };
            arrput(tel.data, td);

            int numLaps = arrlen(tel.idxLap);
            if (prevLap < _laps) {
                arrput(tel.idxLap, idx);
                printf("%ld\n", tel.idxLap[numLaps-1]);

                /* check if best lap time */
                // calculate last lap's time
                printf("%f\n", calcLapTime(tel, prevLap));
                printf("%f\n", calcLapTime(tel, tel.bestLap));


                prevLap = _laps;
            }
        }
//        printf("%f\n", tel.lap0Time);
    }

    fclose(file);

    exit(EXIT_SUCCESS);
}
