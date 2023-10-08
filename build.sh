#！/bin/bash

DIR=`pwd`
if [ -d build ]; then
  rm -rf build
fi

mkdir build

cd build
cmake .. 
make -j4
echo "Build Done"

cd $DIR

