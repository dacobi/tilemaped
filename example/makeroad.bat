del VERAROAD.PRG
cl65 -O -t cx16 veraroad.c  -o VERAROAD.PRG

copy roadmap\map.bin MAP1.BIN
copy roadtmap\tiles.bin TILES.BIN
copy roadtmap\pal.bin PAL.BIN
copy roadmap\map0.bin MAP0.BIN

x16emu  -prg VERAROAD.PRG -run
