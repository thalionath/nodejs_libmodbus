#!/bin/sh

git clone https://github.com/stephane/libmodbus.git

cd ./libmodbus/

# reset to patch commit number
git checkout 60adc0506666d19f9d8bd37bde8d8a728124047f

#patch -c -p2 < ../mt.patch
git apply ../mt.patch

#./autogen.sh && ./configure --enable-static=yes --enable-shared=no && make
./autogen.sh && ./configure && make
