del VERA4.PRG
cl65 -O -t cx16 vera4bpp.c  -o VERA4.PRG

copy tmappot\map0.bin MAP0.BIN
copy tmappot\tiles.bin TILES.BIN
copy tmappot\pal.bin PAL.BIN

x16emu  -prg VERA4.PRG -run
