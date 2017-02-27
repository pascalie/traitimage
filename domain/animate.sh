#!/bin/sh
./scroll 51 0 ../data/earth.ppm tmp1.ppm
./sphere tmp1.ppm gif1.ppm
convert gif1.ppm gif1.png

./scroll 51 0 tmp1.ppm tmp2.ppm
./sphere tmp2.ppm gif2.ppm
convert gif2.ppm gif2.png

./scroll 51 0 tmp2.ppm tmp3.ppm
./sphere tmp3.ppm gif3.ppm
convert gif3.ppm gif3.png

./scroll 51 0 tmp3.ppm tmp4.ppm
./sphere tmp4.ppm gif4.ppm
convert gif4.ppm gif4.png

./scroll 51 0 tmp4.ppm tmp5.ppm
./sphere tmp5.ppm gif5.ppm
convert gif5.ppm gif5.png

./scroll 51 0 tmp5.ppm tmp6.ppm
./sphere tmp6.ppm gif6.ppm
convert gif6.ppm gif6.png

./scroll 51 0 tmp6.ppm tmp7.ppm
./sphere tmp7.ppm gif7.ppm
convert gif7.ppm gif7.png

./scroll 51 0 tmp7.ppm tmp8.ppm
./sphere tmp8.ppm gif8.ppm
convert gif8.ppm gif8.png

./scroll 51 0 tmp8.ppm tmp9.ppm
./sphere tmp9.ppm gif9.ppm
convert gif9.ppm gif9.png

./scroll 51 0 tmp9.ppm tmp10.ppm
./sphere tmp10.ppm gif10.ppm
convert gif10.ppm gif10.png

convert -delay 50 gif1.png gif2.png gif3.png gif4.png gif5.png gif6.png gif7.png gif8.png gif9.png gif10.png -loop 0 test.gif

exit 0