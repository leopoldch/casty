#!/bin/bash

# verify script is running as root
if [ "$EUID" -ne 0 ]
  then echo "Please run as root !"
  exit
fi

# Install the required packages
echo "Installing the required packages..."
apt-get install libbluetooth-dev
 
# Build the project
echo "Building the project..."
mkdir build
cd build
cmake ..
make
cd ..
cp build/castyd .