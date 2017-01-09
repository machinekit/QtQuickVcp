#!/usr/bin/env bash

set -e # Halt on errors
set -x # Be verbose

##########################################################################
# GET DEPENDENCIES
##########################################################################

# select fastet mirror
apt-get update
apt-get install -y netselect-apt
netselect-apt
mv sources.list /etc/apt/sources.list
apt-get clean

# add Machinekit repository
apt-key adv --keyserver keyserver.ubuntu.com --recv 43DDF224
sh -c \
   "echo 'deb http://deb.machinekit.io/debian jessie main' > \
    /etc/apt/sources.list.d/machinekit.list"
apt update
# basic dependencies (needed by Docker image)
apt install -y git wget automake unzip gcc g++ binutils bzip2
# QtQuickVcp's dependencies:
apt-get install -y pkg-config protobuf-compiler
apt-get install -y build-essential gdb dh-autoreconf libgl1-mesa-dev libxslt1.1 git
# dependencies of qmlplugindump
apt-get install -y libfontconfig1 libxrender1 libdbus-1-3 libegl1-mesa
# Android dependencies
apt install -y libtool-bin make curl file libgtest-dev python default-jdk ant lib32z1 lib32ncurses5 lib32stdc++6 python-pip

# install android-publish
pip install -q google-api-python-client
curl -fsSL -o android-publish https://gist.githubusercontent.com/machinekoder/2137bc2ebabfb3fb8daadc1f431e21a5/raw/de171f7c228dd6b14b981cddb3662a0d86cc53ec/android-publish.py
chmod +x android-publish
mv android-publish /usr/bin/


# install Qt-Deployment-Scripts
 [ -d "Qt-Deployment-Scripts" ] || git clone --depth 1 https://github.com/machinekoder/Qt-Deployment-Scripts.git
 cd Qt-Deployment-Scripts
 make install
 cd ..

# download Qt
mkdir -p qt5 && wget -q -O qt5.tar.bz2 http://ci.roessler.systems/files/qt-bin/Qt-5.7-Android-armv7.tar.bz2
tar xjf qt5.tar.bz2 -C qt5
rm qt5.tar.bz2

# download Android NDK
mkdir -p android-ndk && wget -q -O android-ndk.zip https://dl.google.com/android/repository/android-ndk-r12b-linux-x86_64.zip
unzip -qq android-ndk.zip -d android-ndk
rm android-ndk.zip
cd android-ndk
mv */* .
cd ..

# download Android SDK
#mkdir -p android-sdk && wget -q -O android-sdk.tgz https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
#tar xfz android-sdk.tgz -C android-sdk
#rm android-sdk.tgz
mkdir -p android-sdk && wget -q -O android-sdk.tar.bz2 http://ci.roessler.systems/files/qt-bin/android-sdk.tar.bz2
tar xjf android-sdk.tar.bz2 -C android-sdk
cd android-sdk
mv */* .
cd ..

# QtQuickVcp dependencies

# Prepare Android toolchain
./android-ndk/build/tools/make-standalone-toolchain.sh --install-dir=/opt/android-toolchain --arch=arm
export PATH=/opt/android-toolchain/bin:$PATH

# Build ZeroMQ for Android
mkdir -p tmp
cd tmp/

export OUTPUT_DIR=/opt/zeromq-android
export RANLIB=/opt/android-toolchain/bin/arm-linux-androideabi-ranlib

[ -d "zeromq4-x" ] || git clone https://github.com/zeromq/zeromq4-x.git
cd zeromq4-x/
git checkout v4.0.8

# fix compile problems
mv tools/curve_keygen.c tools/curve_keygen.cpp
sed -i 's/\.c\>/&pp/' tools/Makefile.am
rm -f tools/.deps/curve_keygen.Po

./autogen.sh
./configure --enable-static --disable-shared --host=arm-linux-androideabi --prefix=$OUTPUT_DIR \
LDFLAGS="-L$OUTPUT_DIR/lib" CPPFLAGS="-fPIC -I$OUTPUT_DIR/include" LIBS="-lgcc"
make
make install

cd ..

# build Protobuf for Android
export PATH=/opt/android-toolchain/bin:$PATH
export CFLAGS="-fPIC -DANDROID -nostdlib"
export CC=arm-linux-androideabi-gcc
export CXX=arm-linux-androideabi-g++
export NDK=~/bin/android-ndk
export SYSROOT=$NDK/platform/android-9/arch-arm
export OUTPUT_DIR=/opt/protobuf-android

[ -d "protobuf" ] || git clone https://github.com/google/protobuf.git
cd protobuf
git checkout v2.6.1

# trick that outdated autogen script
mkdir -p gtest/msvc
touch gtest/msvc/foo.vcproj
./autogen.sh
./configure --enable-static --disable-shared --host=arm-eabi --with-sysroot=$SYSROOT CC=$CC CXX=$CXX --enable-cross-compile --with-protoc=protoc LIBS="-lc" --prefix=$OUTPUT_DIR
make
make install

# back to tmp
cd ..

# back to root dir
cd ..

# mark image as prepared
touch /etc/system-prepared
