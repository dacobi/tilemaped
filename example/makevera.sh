rm VERA.PRG
cl65 -O -t cx16 vera.c  -o VERA.PRG

cp tmap/map.bin MAP1.BIN
cp tmap/tiles.bin TILES1.BIN
cp tmap/pal.bin PAL.BIN
cp tmap0/map.bin MAP0.BIN
cp tmap0/tiles.bin TILES0.BIN

./x16emu  -prg VERA.PRG -run
