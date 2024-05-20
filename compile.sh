#!/bin/bash

if [ -d "./build" ]; then
  echo "Removing existing build directory..."
  rm -r -d "./build"
fi

echo "Creating build directory..."
mkdir "./build"

echo "Build directory created successfully!"

cd ./build

cmake ..

make -j 7

make install

cp ./build/Seeker ../install/Seeker
rm ./Seeker

echo -e "=========================================================="
echo -e "                                                          "
echo -e "                                                          "
echo -e "\033[32m            [success] finish compile and install \033[0m"
echo -e "                                                          "
echo -e "\033[32m           please enter ./install and run ./Seeker \033[0m"
echo -e "                                                          "
echo -e "                                                          "
echo -e "=========================================================="

