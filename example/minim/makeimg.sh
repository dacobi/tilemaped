#!/bin/bash
./mountimg
rm mnt/MINIM.PRG
rm mnt/DATA/*
cp MINIM.PRG mnt/
cp DATA/* mnt/DATA/
umount mnt
rm minim.zip
7z a minim.zip minim.img
