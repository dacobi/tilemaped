rm VERA4.PRG
cl65 -O -t cx16 vera4bpp.c  -o VERA4.PRG

cp tmappot/map0.bin MAP0.BIN
cp tmappot/tiles.bin TILES.BIN
cp tmappot/pal.bin PAL.BIN

./x16emu  -prg VERA4.PRG -run
