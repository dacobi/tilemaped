del VERAROAD.PRG
cl65 -O -t cx16 veraroad.c  -o VERAROAD.PRG

copy roadmap\map0.bin MAP0.BIN
copy roadmap\tiles.bin TILES.BIN
copy roadmap\pal.bin PAL.BIN
copy roadmap\map1.bin MAP1.BIN

x16emu  -prg VERAROAD.PRG -run
