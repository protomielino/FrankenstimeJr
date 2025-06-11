# FrankenstimeJr
backup

needed libraries:
```
  stb_ds.h (dynamic arrays)
  GTK 3 (windows and such)
  cairo (raw graphic)
  <dependencies> (depending on the system)
```

compile with: `gcc -o telemetry main.c telemetry.c $(pkg-config --cflags --libs gtk+-3.0 cairo) -lm`

run with: `./telemetry`
