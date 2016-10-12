#!/bin/bash

set -x

# install mac ports
# MACPORTS_VERSION=2.3.4
# wget https://distfiles.macports.org/MacPorts/MacPorts-${MACPORTS_VERSION}.tar.bz2
# tar xfj MacPorts-${MACPORTS_VERSION}.tar.bz2
# cd MacPorts-${MACPORTS_VERSION}
# ./configure && sudo make install
# export PATH=/opt/local/bin:$PATH
# cd ..
# sudo port selfupdate
# sudo port install libtool automake m4 autoconf pkgconfig

brew update
brew install libtool automake autoconf pkg-config

# install zeromq
git clone https://github.com/zeromq/zeromq4-x.git
cd zeromq4-x
git checkout v4.0.8
sh autogen.sh
./configure --disable-static --enable-shared --prefix=/opt/local CC=clang CXX=clang++ CFLAGS="-arch x86_64" CXXFLAGS="-std=c++11 -stdlib=libstdc++ -O3 -arch x86_64" LDFLAGS="-stdlib=libstdc++"
make
sudo make install
cd ..

# install protobuf
git clone https://github.com/google/protobuf.git
cd protobuf
git checkout v2.6.1

# trick that outdated autogen script
mkdir -p gtest/msvc
touch gtest/msvc/foo.vcproj
./autogen.sh
./configure --disable-shared --enable-static --prefix=/opt/local CC=clang CXX=clang++ CFLAGS="-arch x86_64" CXXFLAGS="-std=c++11 -stdlib=libstdc++ -O3 -arch x86_64" LDFLAGS="-stdlib=libstdc++"
make
sudo make install
cd ..

#install Qt
which -s qmake
QT_INSTALLED=$?
QMAKE_VERSION=
if [[ $QT_INSTALLED == 0 ]]; then
  QMAKE_VERSION=$(qmake -query QT_VERSION)
fi

echo "QMAKE_VERSION $QMAKE_VERSION"
echo "QT_INSTALLED $QT_INSTALLED"
echo "QT_LONG_VERSION QT_LONG_VERSION"

if [[ "$QMAKE_VERSION" != "${QT_LONG_VERSION}" ]]; then
  rm -rf $QT_PATH
  echo "Downloading Qt"
  wget -c --no-check-certificate -nv https://download.qt.io/archive/qt/${QT_SHORT_VERSION}/${QT_LONG_VERSION}/${QT_INSTALLER_FILENAME}
  hdiutil mount ${QT_INSTALLER_FILENAME}
  cp -rf /Volumes/${QT_INSTALLER_ROOT}/${QT_INSTALLER_ROOT}.app $HOME/${QT_INSTALLER_ROOT}.app
  QT_INSTALLER_EXE=$HOME/${QT_INSTALLER_ROOT}.app/Contents/MacOS/${QT_INSTALLER_ROOT}

  echo "Installing Qt"
  ./build/travis/job_macos/extract-qt-installer $QT_INSTALLER_EXE $QT_PATH
  rm -rf $HOME/${QT_INSTALLER_ROOT}.app
else
  echo "Qt ${QT_LONG_VERSION} already installed"
fi











