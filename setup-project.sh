#!/bin/bash
git submodule init
git submodule update

#Setup nzmqt
cd ./3rdparty/nzmqt/
./setup-project.sh
cd ../../

# Setup project directories.
#mkdir -p externals/include

# Copy files.
#cp externals-src/cppzmq/zmq.hpp externals/include

