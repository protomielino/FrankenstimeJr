# FrankenstimeJr
backup

needed libraries:
```
  stb_ds.h (dynamic arrays)
  GTK 3 (windows and such)
  cairo (raw graphic)
  <dependencies> (depending on the system)
```

  `LSHIFT` + `SCROLL` = zoom time
  
  `LCTRL` + `SCROLL` = zoom distance
  
  `CLICK` + `DRAG` = pan
  
  `LSHIFT` + `CLICK` + `DRAG` = select
  

compile with: `gcc -o telemetry main.c telemetry.c $(pkg-config --cflags --libs gtk+-3.0 cairo) -lm`

run with: `./telemetry <csv_file> <lap_number> <dump_lap[0..1]>`
