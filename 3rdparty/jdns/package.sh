#!/bin/sh
set -e

if [ $# -lt 1 ]; then
	echo "usage: $0 [version]"
	exit 1
fi

VERSION=$1

mkdir -p build/jdns-$VERSION
cp -a .gitignore COPYING README.md TODO CMakeLists.txt cmake_uninstall.cmake.in jdns.* qjdns.* JDns* QJDns* include src tools build/jdns-$VERSION
cd build
tar jcvf jdns-$VERSION.tar.bz2 jdns-$VERSION
