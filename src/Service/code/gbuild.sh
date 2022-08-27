#!/bin/bash
echo "build start..."
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .

if [ $1 == "-exe" ];  
then  
echo "execute the building result..."
./service
else
echo "build finish"
fi