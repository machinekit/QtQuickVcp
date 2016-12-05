#!/usr/bin/env bash

set -e # Halt on errors
set -x # Be verbose

##########################################################################
# GET DEPENDENCIES
##########################################################################

# add Machinekit repository
apt-key adv --keyserver keyserver.ubuntu.com --recv 43DDF224
sh -c \
   "echo 'deb http://deb.machinekit.io/debian jessie main' > \
    /etc/apt/sources.list.d/machinekit.list"
apt update
# basic dependencies (needed by Docker image)
apt install -y git wget automake unzip gcc g++ binutils bzip2
# QtQuickVcp's dependencies:
apt-get install -y pkg-config libprotobuf-dev protobuf-compiler libzmq3-dev
apt-get install -y build-essential gdb dh-autoreconf libgl1-mesa-dev libxslt1.1 git
# dependencies of qmlplugindump
apt-get install -y libfontconfig1 libxrender1 libdbus-1-3 libegl1-mesa


# Build AppImageKit now to avoid conflicts with MuseScore's dependencies (LAME)
[ -d "AppImageKit" ] || git clone --depth 1 https://github.com/probonopd/AppImageKit.git
cd AppImageKit
./build.sh

 cd ..

 [ -d "Qt-Deployment-Scripts" ] || git clone --depth 1 https://github.com/machinekoder/Qt-Deployment-Scripts.git
 cd Qt-Deployment-Scripts
 make install
 cd ..

# install Qt
mkdir -p qt5 && wget -q -O qt5.tar.bz2 http://buildbot.roessler.systems/files/qt-bin/Qt-5.7-Linux-x64.tar.bz2
tar xjf qt5.tar.bz2 -C qt5
rm qt5.tar.bz2

# mark image as prepared
touch /etc/system-prepared
