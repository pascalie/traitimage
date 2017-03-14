#!/bin/sh
./colorization ../data/sat-c.ppm ../data/sat-g.ppm a.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm b.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm c.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm d.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm e.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm f.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm g.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm h.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm i.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm j.ppm
./colorization ../data/sat-c.ppm ../data/sat-g.ppm k.ppm

pvisu *.ppm


exit 0