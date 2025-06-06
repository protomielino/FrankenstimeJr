# FrankenstimeJr
backup

compile with:
gcc -o graph graph.c $(pkg-config --cflags --libs gtk+-3.0 cairo) -lm
and
gcc -o telemetry telemetry.c $(pkg-config --cflags --libs gtk+-3.0 cairo) -lm
