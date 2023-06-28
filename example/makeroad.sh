rm VERAROAD.PRG
cl65 -O -t cx16 veraroad.c  -o VERAROAD.PRG

cp roadmap/map0.bin MAP0.BIN
cp roadmap/map1.bin MAP1.BIN
cp roadmap/tiles.bin TILES.BIN
cp roadmap/pal.bin PAL.BIN

./x16emu  -prg VERAROAD.PRG -run
