#!/bin/sh
./pipeline ../data/test-01.ppm test1.ppm
./pipeline ../data/test-02.ppm test2.ppm
./pipeline ../data/test-03.ppm test3.ppm
./pipeline ../data/test-04.ppm test4.ppm
./pipeline ../data/test-05.ppm test5.ppm
./pipeline ../data/test-06.ppm test6.ppm
./pipeline ../data/test-07.ppm test7.ppm
./pipeline ../data/test-08.ppm test8.ppm

pvisu *.ppm


exit 0