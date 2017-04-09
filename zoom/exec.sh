#!/bin/sh
./copy 10 ../data/cameraman.ppm copy.ppm
./padding 10 ../data/cameraman.ppm padding.ppm
./filter 15 box ../data/cameraman.ppm box.ppm
./filter 15 tent ../data/cameraman.ppm tent.ppm
./filter 15 bell ../data/cameraman.ppm bell.ppm
./filter 15 mitch ../data/cameraman.ppm mitch.ppm

pvisu *.ppm


exit 0