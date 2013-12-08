#!/bin/bash

clear

source ~/Desktop/SharedFolder/trimaran/scripts/envrc.bash

echo "Compiling openimpact..."
cd ~/Desktop/SharedFolder/trimaran/openimpact
make
echo "Done!"

echo "Running benchmark..."
cd ~/Desktop/test
tcc -bench $1 -region h > output.txt
echo "Done!"

gedit output.txt
