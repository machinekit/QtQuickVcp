#!/usr/bin/env bash
# Run this script from the projects root directory
set -e

BASEDIR=${PWD}
DISTDIR=${BASEDIR}/dist
BUILDDIR=docker-build
PROJECTDIR=QtQuickVcp

cd ..
rm -rf ${BUILDDIR}
cp -r ${PROJECTDIR} ${BUILDDIR}
cd ${BUILDDIR}
git clean -fxd
./build/docker/AppImage/build.sh
mkdir -p ${DISTDIR}
mv *.AppImage ${DISTDIR}
mv *.tar.gz ${DISTDIR}
cd ..
rm -rf ${BUILDDIR}
