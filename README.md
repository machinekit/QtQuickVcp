QtQuickVcp
==========

A remote component implementation for HAL written in Qt/C++/QML.

This application is intended to be used demonstration for the new HAL remote components. The application was tested on Desktop Linux and Android.

Install/Compile
---------------

Prerequirements
------
Install Qt SDK for Android
Download and extract Android NDK and SDK

ZeroMQ on Android
------
 http://zeromq.org/build:android
 Compile fix: http://stackoverflow.com/questions/21017296/how-to-build-zeromq-on-windows-with-cygwin

Alter and execute the following commands
--
sudo ./android-ndk-r95/build/tools/make-standalone-toolchain.sh --install-dir=/opt/android-toolchain
export PATH=/opt/android-toolchain/bin:$PATH
export OUTPUT_DIR=/tmp/zeromq-android

cd /tmp/
git clone https://github.com/zeromq/zeromq4-x.git
cd zeromq4-x/
mv tools/curve_keygen.c tools/curve_keygen.cpp
sed -i 's/\.c\>/&pp/' tools/Makefile.am
rm -f tools/.deps/curve_keygen.Po
./autogen.sh
./configure --enable-static --disable-shared --host=arm-linux-androideabi --prefix=$OUTPUT_DIR LDFLAGS="-L$OUTPUT_DIR/lib" CPPFLAGS="-fPIC -I$OUTPUT_DIR/include" LIBS="-lgcc"
make
make install

nzmqt
------
run ./setup_project.sh

protobuf on Android
------
svn checkout http://protobuf.googlecode.com/svn/trunk/ protobuf-read-only
cd protobuf-read-only

export PATH=/opt/android-toolchain/bin:$PATH
export CFLAGS="-fPIC -DANDROID -nostdlib"
export CC=arm-linux-androideabi-gcc
export CXX=arm-linux-androideabi-g++
export NDK=~/bin/android-ndk
export SYSROOT=$NDK/platform/android-9/arch-arm

./autogen.sh
./configure --host=arm-eabi --with-sysroot=$SYSROOT CC=$CC CXX=$CXX --enable-cross-compile --with-protoc=protoc LIBS="-lc"
make

iOS install
------
install XCode from the App Store
install MacPorts: http://www.macports.org/install.php
sudo port selfupdate
sudo port install libtool
sudo port install automake
sudo port install m4
sudo port install autoconf

ZeroMQ on iOS
------
https://github.com/drewcrawford/libzmq-ios
Use the precompiled binaries

protobuf on iOS
------
https://gist.github.com/strahlex/10585771
