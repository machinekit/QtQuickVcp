#!/usr/bin/env bash
# Run this script from the projects root directory
set -e

BASEDIR=${PWD}
BUILDDIR=docker-build
PROJECTDIR=QtQuickVcp

cd ..
rm -rf ${BUILDDIR}
cp -r ${PROJECTDIR} ${BUILDDIR}
cd ${BUILDDIR}
git clean -fxd
./build/docker/Test/build.sh
cd ..
rm -rf ${BUILDDIR}
