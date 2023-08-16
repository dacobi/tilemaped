#!/bin/bash
split $1 SND -b 8192 -a 4 --numeric-suffixes=1 --additional-suffix=.BIN
