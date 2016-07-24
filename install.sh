#!/bin/bash

############## Installation script ###############

#Perform makefile operations and other compilation processes
echo "Configuring..."
cd src/
make
make clean
cd ..
echo "SUCCESS"

#Install to the system
echo "Installing..."
cp -rf bin/ishell /usr/bin/
echo "SUCCESS"

exit
