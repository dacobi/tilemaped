#!/bin/bash
# The Gimp X16 palette is from this site: https://www.8bitcoding.com/2019/12/default-palette.html, created by Jimmy Dansbo, with code from FlyingLow
./palformat.sh $1 > newpalette.h
g++ palgen.cpp -o palgen
./palgen $2
