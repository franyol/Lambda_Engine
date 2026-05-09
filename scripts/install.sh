#!/bin/bash

cmake -DCMAKE_INSTALL_PREFIX=/usr/local -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cp build/compile_commands.json ./compile_commands.json

cd build
sudo make install
