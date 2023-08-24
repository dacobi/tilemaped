#!/bin/bash
./mountimg
rm mnt/MINIM.PRG
rm mnt/DAT/*
cp MINIM.PRG mnt/
cp DAT/* mnt/DAT/
umount mnt
rm minim.zip
7z a minim.zip minim.img
