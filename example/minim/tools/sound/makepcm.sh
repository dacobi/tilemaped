./tools/clean.sh
./tools/splitpcm.sh $1
./tools/swab.sh
./tools/convert.sh
rm SND*
